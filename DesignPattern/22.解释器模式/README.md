# 解释器模式

解释器模式（Interpreter Pattern）是⼀种⾏为型设计模式，它定义了⼀个语⾔的⽂法，并且建⽴⼀个【解释器】来解释该语⾔中的句⼦。

⽐如说SQL语法、正则表达式，这些内容⽐较简短，但是表达的内容可不仅仅是字⾯上的那些符号，
计算机想要理解这些语法，就需要解释这个语法规则，因此解释器模式常⽤于实现编程语⾔解释器、正则表达式处理等场景。

解释器模式主要包含以下⼏个⻆⾊：
1. 抽象表达式（Abstract Expression）：定义了解释器的接⼝，包含了解释器的⽅法interpret。
2. 终结符表达式（Terminal Expression）：在语法中不能再分解为更⼩单元的符号。
3. ⾮终结符表达式（Non-terminal Expression）：⽂法中的复杂表达式，它由终结符和其他⾮终结符组成。
4. 上下⽂（Context）：包含解释器之外的⼀些全局信息，可以存储解释器中间结果，也可以⽤于向解释器传递信息。

当需要解释和执⾏特定领域或业务规则的语⾔时，可以使⽤解释器模式。例如，SQL解释器、正则表达式解释器等。
但是需要注意的是解释器模式可能会导致类的层次结构较为复杂，同时也可能不够灵活，使⽤要慎重。
