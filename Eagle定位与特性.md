# Eagle定位与特性

### 定位
* 类Python的动态类型，解释型语言
* 具有面向过程（OP），面向对象（OO）和部分函数式特性
* 实现基础表达式，控制流，面向对象，继承
* 内置list和map两种迭代器

### 特性
* 引入大括号`{}`和分号`;`来区分代码块
* 引入三目运算符
```
var a = condition ? t_value : f_value;
```

* 引入高阶函数map/reduce/filter
  * filter example

```eagle
def check(n) {
    return n > 2;
}

var l = [1, 2, 3, 4, 5];
print l.filter(check); // [3, 4, 5]

var d = {1: "1", 2: "2", 3: "3"};
print d.values.filter(check); // {3: "3"}
print d.keys.filter(check); // error?
```

* 引入match/switch case模式匹配
```eagle
var a = switch (condition) {
    case c1 => v1;
    case c2 => v2;
    case _  => v3;
};
```



### 命令行交互
* `cmd`
* 多行交互时，通过cmd先缓存输入，然后进行字符串匹配，比如括号匹配，括号匹配都正确了，压缩成一行进入解释器？
```
$ > class A {
  | 
  | }
$ > def func() {
  | 
  | }
```
* 表达式都提供返回值，这样计算表达式可以直接运算？
