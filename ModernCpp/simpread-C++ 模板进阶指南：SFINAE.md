# C++模板进阶指南：SFINAE

原文地址 [知乎-空明流转](https://zhuanlan.zhihu.com/p/21314708)

SFINAE 可以说是 C++ 模板进阶的门槛之一，如果选择一个论题来测试对 C++ 模板机制的熟悉程度，那么在我这里，首选就应当是 SFINAE 机制。

我们不用纠结这个词的发音，它来自于 Substitution failure is not an error 的首字母缩写。这一句之乎者也般难懂的话，由之乎者 —— 啊，不，Substitution，Failure 和 Error 三个词构成。

我们从最简单的词 “Error” 开始理解。Error 就是一般意义上的编译错误。一旦出现编译错误，大家都知道，编译器就会中止编译，并且停止接下来的代码生成和链接等后续活动。

其次，我们再说 “Failure”。很多时候光看字面意思，很多人会把 Failure 和 Error 等同起来。但是实际上 Failure 很多场合下只是一个**中性词**。比如我们看下面这个虚构的例子就知道这两者的区别了。

假设我们有一个语法分析器，其中某一个规则需要匹配一个 token，它可以是标识符，字面量或者是字符串，那么我们会有下面的代码：

```
switch(token)
{
case IDENTIFIER:
    // do something
    break;
case LITERAL_NUMBER:
    // do something
    break;
case LITERAL_STRING:
    // do something
    break;
default:
    throw WrongToken(token);
}


```

假如我们当前的 token 是 LITERAL_STRING 的时候，那么第一步，它在匹配 IDENTIFIER 时，我们可以认为它 **failure** 了。

但是如果这个 Token 既不是标识符，也不是数字字面量，也不是字符串字面量的时候，并且，我们的语法规则认为这一条件是无论如何都不可接受的，这时我们就认为它是一个 **error**。

比如大家所熟知的函数重载，也是如此。比如说下面这个例子：

```
struct A {};
struct B: public A {};
struct C {};

void foo(A const&) {}
void foo(B const&) {}

void callFoo() {
  foo( A() );
  foo( B() );
  foo( C() );
}


```

那么 foo(A() ) 虽然匹配 foo(B const&) 会失败，但是它起码能匹配 foo(A const&)，所以它是正确的； foo( B() ) 能同时匹配两个函数原型，但是 B & 要更好一些，因此它选择了 B。而 foo( C() ); 因为两个函数都匹配失败（**Failure**）了，所以它找不到相应的原型，这时才会爆出一个编译器错误（**Error**）。

所以到这里我们就明白了，**在很多情况下，**Failure is not an error，因为编译器在遇到 Failure 的时候，往往还需要尝试其他的可能性。

好，现在我们把最后一个词，Substitution，加入到我们的字典中。现在这句话的意思就是说，我们要把 Failure is not an error 的概念，推广到 Substitution 阶段。

所谓 substitution，就是将函数模板中的形参，替换成实参的过程。C++ 标准中对这一概念的解释比较拗口，它分别指出了以下几点：

*   什么时候函数模板会发生实参 替代（Substitute） 形参的行为
*   什么样的行为被称作 Substitution  
    
*   什么样的行为**不**可以被称作 Substitution Failure —— 他们叫 SFINAE error。

我们在此不再详述，有兴趣的同学可以参照 [SFINAE - cppreference.com](https://link.zhihu.com/?target=http%3A//en.cppreference.com/w/cpp/language/sfinae) ，这是标准的一个精炼版本。如果只总结最常见的情况，那就是假设我们有这么个函数签名：

```
template <
  typename T0, 
  // 一大坨其他模板参数
  typename U = /* 和前面T有关的一大坨 */
>
RType /* 和模板参数有关的一大坨 */
functionName (
   PType0 /* PType0 是和模板参数有关的一大坨 */,
   PType1 /* PType1 是和模板参数有关的一大坨 */,
   // ... 其他参数
) {
  // 实现，和模板参数有关的一大坨
}


```

那么，所有函数签名上的 “和模板参数有关的一大坨”，基本都是 Substitution 时要处理的东西（当然也有一些例外）。一个更具体的例子来解释上面的 “一大坨”：

```
template <
  typename T, 
  typenname U = typename vector<T>::iterator // 1
>
typename vector<T>::value_type  // 1
  foo( 
      T*, // 1
      T&, // 1
      typename T::internal_type, // 1
      typename add_reference<T>::type, // 1
      int // 这里都不需要 substitution
  )
{
   // 整个实现部分，都没有 substitution。这个很关键。
}


```

嗯，粗糙的介绍完 SFINAE 之后，我们先来看一个最常见的例子看看它是什么个行为：  

```
struct X {
  typedef int type;
};

struct Y {
  typedef int type2;
};

template <typename T> void foo(typename T::type);    // Foo0
template <typename T> void foo(typename T::type2);   // Foo1
template <typename T> void foo(T);                   // Foo2

void callFoo() {
   foo<X>(5);    // Foo0: Succeed, Foo1: Failed,  Foo2: Failed
   foo<Y>(10);   // Foo0: Failed,  Foo1: Succeed, Foo2: Failed
   foo<int>(15); // Foo0: Failed,  Foo1: Failed,  Foo2: Succeed
}


```

在这个例子中，当我们指定 foo<Y> 的时候，substitution 就开始工作了，而且会同时工作在三个不同的 foo 签名上。如果我们仅仅因为 Y 没有 type，就在匹配 Foo0 时宣布出错，那显然是武断的，因为我们起码能保证，也希望将这个函数匹配到 Foo1 上。

实际上，std/boost 库中的 enable_if 也是借用了这个原理。

我们来看 enable_if 的一个应用：假设我们有两个不同类型的 counter，一种 counter 是普通的整数类型，另外一种 counter 是一个复杂对象，它有一个成员叫做 increase。现在，我们想把这两种类型的 counter 封装一个统一的调用：inc_counter。那么，我们直觉会简单粗暴的写出下面的代码：

```
struct ICounter {
  virtual void increase() = 0;
  virtual ~ICounter() {}
};

struct Counter: public ICounter {
   void increase() override {
      // Implements
   }
};

template <typename T>
void inc_counter(T& counterObj) {
  counterObj.increase();
}

template <typename T>
void inc_counter(T& intTypeCounter){
  ++intTypeCounter;
}

void doSomething() {
  Counter cntObj;
  uint32_t cntUI32;

  // blah blah blah
  inc_counter(cntObj);
  inc_counter(cntUI32);
}


```

我们非常希望它可以如我们所愿的 work —— 因为其实我们是知道对于任何一个调用，两个 inc_counter 只有一个是正常工作的。“**有且唯一**”，我们理应当期望编译器能够挑出那个唯一来。

可惜编译器做不到这一点。首先，它就告诉我们，这两个签名其实是一模一样的，我们遇到了 redefinition。

```
template <typename T> void inc_counter(T& counterObj);
template <typename T> void inc_counter(T& intTypeCounter);


```

所以我们要借助于 enable_if 这个 T 对于不同的实例做个限定：

```
template <typename T> void inc_counter(
  T& counterObj, 
  typename std::enable_if<
    is_base_of<T, ICounter>::value
  >::type* = nullptr );

template <typename T> void inc_counter(
  T& counterInt,
  typename std::enable_if<
    std::is_integral<T>::value
  >::type* = nullptr );


```

关于这个 enable_if 是怎么工作的，语法为什么这么丑，我来解释一下：

首先，substitution 只有在推断函数类型的时候，才会起作用。推断函数类型需要参数的类型，所以，typename std::enable_if<std::is_integral<T>::value>::type 这么一长串代码，就是为了让 enable_if 参与到函数类型中；

其次，is_integral<T>::value 返回一个布尔类型的编译器常数，告诉我们它是或者不是一个 integral，enable_if<C > 的作用就是，如果这个 C 值为 True，那么 type 就会被推断成一个 void 或者是别的什么类型，让整个函数匹配后的类型变成 void inc_counter<int>(int & counterInt, void* dummy = nullptr); 如果这个值为 False，那么 enable_if<false > 这个特化形式中，压根就没有这个::type，于是 substitution 就失败了 —— 所以这个函数原型根本就不会被产生出来。

所以我们能保证，无论对于 int 还是 counter 类型的实例，我们都只有一个函数原型是通过了 substitution —— 这样就保证了它的 “有且唯一”，编译器也不会因为你某个替换失败而无视成功的那个实例。

这个例子说到了这里，熟悉 C++ 的你，一定会站出来说我们只要把第一个签名改成如下的形式：

```
void inc_counter(ICounter& counterObj);


```

就能完美解决这个问题了，根本不需要这么复杂的编译器机制。

嗯，你说的没错，在这里这个特性一点都没用。

这也提醒我们，当你觉得需要写 enable_if 的时候，首先要考虑到以下可能性：

*   重载（对模板函数）  
    
*   偏特化（对模板类而言）  
    
*   虚函数

但是问题到了这里并没有结束。因为，increase 毕竟是个虚函数。假如 counter 需要调用的地方实在是太多了，这个时候我们会非常期望 increase 不再是个虚函数以提高性能。此时我们会调整继承层级：  

```
struct ICounter {};
struct Counter: public ICounter {
  void increase() {
    // impl
  }
};


```

那么原有的 void inc_counter(ICounter& counterObj) 就无法再执行下去了。这个时候你可能会考虑一些变通的办法：  

```
template <typename T>
void inc_counter(ICounter& c) {};

template <typename T>
void inc_counter(T& c) { ++c; };

void doSomething() {
  Counter cntObj;
  uint32_t cntUI32;

  // blah blah blah
  inc_counter(cntObj); // 1
  inc_counter(static_cast<ICounter&>(cntObj)); // 2
  inc_counter(cntUI32); // 3
}


```

对于 1，因为 cntObj 到 ICounter 是需要类型转换的，所以比 void inc_counter(T&) [T = Counter] 要更差一些。然后它会直接实例化后者，结果实现变成了 ++cntObj，BOOM！

那么我们做 2 试试看？嗯，工作的很好。但是等等，我们的初衷是什么来着？不就是让 inc_counter 对不同的计数器类型透明吗？这不是又一夜回到解放前了？

所以这个时候，就能看到 enable_if 是如何通过 SFINAE 发挥威力的了。

那么为什么我们还要 ICounter 作为基类呢？ 这是个好问题。在本例中，我们用它来区分一个 counter 是不是继承自 ICounter。最终目的，是希望知道 counter 有没有 increase 这个函数。

所以 ICounter 只是相当于一个标签。而于情于理这个标签都是个累赘。但是在 C++11 之前，我们并没有办法去写类似于：

```
template <typename T> void foo(T& c, decltype(c.increase())* = nullptr);


```

这样的函数签名，因为加入 T 是 int，那么 c.increase() 这个函数调用并不属于 Type Failure，它是一个 Expression Failure，会导致编译器出错。所以我们才退而求其次，用一个类似于标签的形式来提供足够的信息。

到了 C++11，它正式提供了 Expression SFINAE，这时我们就能抛开 ICounter 这个无用的 Tag，直接写出我们要写的东西：

```
struct Counter {
   void increase() {
      // Implements
   }
};

template <typename T>
void inc_counter(T& intTypeCounter, std::decay_t<decltype(++intTypeCounter)>* = nullptr) {
  ++intTypeCounter;
}

template <typename T>
void inc_counter(T& counterObj, std::decay_t<decltype(counterObj.increase())>* = nullptr) {
  counterObj.increase();
}

void doSomething() {
  Counter cntObj;
  uint32_t cntUI32;

  // blah blah blah
  inc_counter(cntObj);
  inc_counter(cntUI32);
}


```

此外，还有一种情况只能使用 SFINAE，而无法使用包括继承、重载在内的任何方法，这就是 Universal Reference。比如，

```
// 这里的a是个通用引用，可以准确的处理左右值引用的问题。
template <typename ArgT> void foo(ArgT&& a);


```

加入我们要限定 ArgT 只能是 float 的衍生类型，那么写成下面这个样子是不对的，它实际上只能接受 float 的右值引用。

```
void foo(float&& a);


```

此时的唯一选择，就是使用通用引用，并增加 enable_if 限定类型，如下面这样：

```
template <typename ArgT>
void foo(
  ArgT&& a, 
  typename std::enabled_if<
    is_same< std::decay_t<ArgT>, float>::value
  >::type* = nullptr
);


```

从上面这些例子可以看到，SFINAE 最主要的作用，是保证编译器在泛型函数、偏特化、及一般重载函数中遴选函数原型的候选列表时不被打断。除此之外，它还有一个很重要的元编程作用就是实现部分的编译期自省和反射。  

虽然它写起来并不直观，但是对于既没有编译器自省、也没有 Concept 的 C++1y 来说，已经是最好的选择了。

稍后会更新到 Github 中。