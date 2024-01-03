# 1.1 OpenGL

现代OpenGL采用核心模式，立即渲染模式已经被废除

## 扩展

扩展依赖于特定的显卡以及驱动，使用方法如下

```c
if(GL_ARB_extension_name)
{
    // 使用硬件支持的全新的现代特性
}
else
{
    // 不支持此扩展: 用旧的方式去做
}
```

## 状态机

OpenGL自身就是一个巨大的状态机，他以这种方式运行

想要修改什么，就是修改不同数据的绑定和内容，也就是修改状态

使用和修改一个OpenGL对象或者状态的流程如下

```c
// 创建对象
unsigned int objectId = 0;
glGenObject(1, &objectId);
// 绑定对象至上下文
glBindObject(GL_WINDOW_TARGET, objectId);
// 设置当前绑定到 GL_WINDOW_TARGET 的对象的一些选项
glSetObjectOption(GL_WINDOW_TARGET, GL_OPTION_WINDOW_WIDTH, 800);
glSetObjectOption(GL_WINDOW_TARGET, GL_OPTION_WINDOW_HEIGHT, 600);
// 将上下文对象设回默认
glBindObject(GL_WINDOW_TARGET, 0);
```

这一小段代码展现了你以后使用OpenGL时常见的工作流。我们首先创建一个对象，然后用一个id保存它的引用（实际数据被储存在后台）。

然后我们将对象绑定至上下文的目标位置（例子中窗口对象目标的位置被定义成`GL_WINDOW_TARGET`）。

接下来我们设置窗口的选项。最后我们将目标位置的对象id设回0，解绑这个对象。设置的选项将被保存在`objectId`所引用的对象中，一旦我们重新绑定这个对象到`GL_WINDOW_TARGET`位置，这些选项就会重新生效。

