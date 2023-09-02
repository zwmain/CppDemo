# C/C++ Socket 网络编程学习

项目在Linux平台实现

CMake构建项目

```shell
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=TRUE
```

- `-S`指定源代码文件夹
- `-B`指定输出目录
- `-DCMAKE_BUILD_TYPE`设置构建类型
- `-DCMAKE_EXPORT_COMPILE_COMMANDS`指定生成`compile_commands.json`文件

CMake编译项目

```shell
cmake --build build --config Debug
```

- `--build`选项指定构建文件夹
- `--config`设置编译类型

