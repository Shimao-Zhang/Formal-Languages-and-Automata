# 编译说明

在 ubuntu-20.04 x64 上使⽤ g++进行编译，本次实验的源代码文件名为turing.cpp，编译生成可执行文件的具体指令为：

```
g++ -o turing turing.cpp
```

生成可执行文件为turing，运行图灵机程序的具体指令格式有两种。

普通模式下为：

```
./turing 图灵机文件名 输入
```

verbose模式下为：

```
./turing -v|--verbose 图灵机文件名 输入
```

