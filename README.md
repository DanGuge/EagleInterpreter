# Eagle Interpreter
* extended python



### 环境

+ 语言：c++11
+ 工具链及最低版本：
	+ g++ 8.0
	+ cmake 3.16
	+ make 4.0



### 使用

#### 编译

+ windows平台：使用CLion等工具进行编译
+ linux / mac平台：使用如下命令

```
git clone git@github.com:DanGuge/EagleInterpreter.git
cd EagleInterpreter
mkdir build && cd build
cmake ..
make
```

#### 运行

Eagle解释器主要通过命令行运行，有两种方法，以linux系统为例

1. 解释执行单个文件，需要附上源代码文件路径作为命令行参数：

```bash
~/EagleInterpreter/build$ cat test.eagle
var a = 1;
print a;
~/EagleInterpreter/build$ ./EagleInterpreter test.eagle
1
```

2. 使用EagleShell，不需要任何命令行参数：

```bash
~/EagleInterpreter/build$ ./EagleInterpreter
>>> var a = 1;
>>> print a;
1
>>> def f() {
...     print "func";
... }
...
>>> f();
func
>>> quit
```

