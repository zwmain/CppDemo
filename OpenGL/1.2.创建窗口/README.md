# 1.2.创建窗口

## GLFW

OpenGL本身无法显示窗口，因此需要借助其它库显示窗口，并和OpenGL结合起来

这里用GLFW，其实用Qt也是可以的，Qt也可以和OpenGL结合起来

在GLFW[下载页](https://www.glfw.org/download.html)下载源码或者二进制库

如果会用CMake直接下载源码就行，不会的就下载二进制文件

会用CMake的话，直接就用CMake管理项目，不用单独编译

## GLAD

因为OpenGL只是一个标准/规范，具体的实现是由驱动开发商针对特定显卡实现的。

由于OpenGL驱动版本众多，它大多数函数的位置都无法在编译时确定下来，需要在运行时查询。

所以任务就落在了开发者身上，开发者需要在运行时获取函数地址并将其保存在一个函数指针中供以后使用。

取得地址的方法因平台而异，在Windows上会是类似这样：

```c
// 定义函数原型
typedef void (*GL_GENBUFFERS) (GLsizei, GLuint*);
// 找到正确的函数并赋值给函数指针
GL_GENBUFFERS glGenBuffers  = (GL_GENBUFFERS)wglGetProcAddress("glGenBuffers");
// 现在函数可以被正常调用了
GLuint buffer;
glGenBuffers(1, &buffer);
```

你可以看到代码非常复杂，而且很繁琐，我们需要对每个可能使用的函数都要重复这个过程。

幸运的是，有些库能简化此过程，其中GLAD是目前最新，也是最流行的库。

配置GLAD
GLAD是一个[开源](https://github.com/Dav1dde/glad)的库，它能解决我们上面提到的那个繁琐的问题。

GLAD的配置与大多数的开源库有些许的不同，GLAD使用了一个[在线服务](http://glad.dav1d.de/)。在这里我们能够告诉GLAD需要定义的OpenGL版本，并且根据这个版本加载所有相关的OpenGL函数。

打开GLAD的[在线服务](http://glad.dav1d.de/)，将语言(Language)设置为C/C++，在API选项中，选择3.3以上的OpenGL(gl)版本（我们的教程中将使用3.3版本，但更新的版本也能用）。之后将模式(Profile)设置为Core，并且保证选中了生成加载器(Generate a loader)选项。现在可以先（暂时）忽略扩展(Extensions)中的内容。都选择完之后，点击生成(Generate)按钮来生成库文件。

GLAD现在应该提供给你了一个zip压缩文件，包含两个头文件目录，和一个glad.c文件。将两个头文件目录（glad和KHR）复制到你的Include文件夹中（或者增加一个额外的项目指向这些目录），并添加glad.c文件到你的工程中。

经过前面的这些步骤之后，你就应该可以将以下的指令加到你的文件顶部了：

```c
#include <glad/glad.h> 
```

## OpenGL

一般情况下，跟随系统附带，无需额外配置


## CMake配置

把GLFW和GLAD都放在third-party目录下

定义一些变量指定到他们的具体目录或者文件

用`add_subdirectory`让CMake能找到GLFW这样使用CMake的库

对于GLFW还要额外连接其生成的库

```cmake
cmake_minimum_required(VERSION 3.18)
project("LearnOpenGL" LANGUAGES C CXX)

set(TRD_DIR ${CMAKE_SOURCE_DIR}/third-party)
set(GLFW_DIR ${TRD_DIR}/glfw-3.3.9)
set(GLAD_DIR ${TRD_DIR}/glad)
set(GLAD_SRC_FILE ${TRD_DIR}/glad/src/glad.c)

add_subdirectory(${GLFW_DIR})


include_directories(${GLFW_DIR}/include)
include_directories(${GLAD_DIR}/include)

link_libraries(glfw)

add_subdirectory("${CMAKE_SOURCE_DIR}/1.3.你好窗口")

```

