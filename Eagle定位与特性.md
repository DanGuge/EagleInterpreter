# Eagle定位与特性

### 定位
* 类Python的动态类型，解释型语言
* 具有面向过程（OP），面向对象（OO）和部分函数式特性
* 实现基础表达式，控制流，面向对象，继承
* 内置list、tuple、dict三种迭代器
* 类缺乏弹性，定义好的类无法额外添加需要和变化

### 特性
* 引入大括号`{}`和分号`;`来区分代码块
* 引入三目运算符
```
var a = condition ? t_value : f_value;
```

* 引入stream流操作（类似java的Stream流）和高阶函数map/reduce/filter
  * filter example

```eagle
def check(n) {
    return n > 2;
}

var l = [1, 2, 3, 4, 5];
print stream(l).filter(check).to_list(); // [3, 4, 5]

var d = {1: "1", 2: "2", 3: "3"};
print stream(d.values()).filter(check).to_list(); // 运行时error, 字符串和整数不能进行比较
print stream(d.keys()).filter(check).to_list(); // [3, ]

def check_item(item) {
	return item[0] >= 2
}

print stream(d.items()).filter(check_item).to_list(); // [(2, "2"), (3, "3")]
print stream(d.items()).filter(check_item).to_dict(); // {2: "2", 3: "3"}
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
