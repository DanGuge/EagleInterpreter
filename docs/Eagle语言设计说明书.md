# Eagle语言设计说明书

吴晨灿 王艺坤

## 1. 语言的背景和目标

1. 设计背景
2. 设计目标
3. 语言名称由来（x）

* 动态类型，解释性语言，考虑了部分跨平台需求
* 参考：Lox，Python，Java，Scala，C++？

## 2. 词法设计

### 2.1 Token设计

在Eagle中，Token包括四个基本成员变量，分别是：

* TokenType type：Token类型标识符
* int line：Token出现的行数，用于错误检查
* string text：Token对应的字符串
* Object literal：当该Token为基本类型，则literal为具体基本类型，否则为nullptr
  * Object为Eagle内置实现的公共父类

### 2.2 TokenType设计

在Eagle，TokenType主要可以分为5大类，分别是算术运算符，关系运算符，字面量标识符，关键字和基本符号，另外还有一个辅助标识符，用于Parser语法解析时辅助使用。

TokenType的作用是区分Lexer词法解析的分析结果，在Parser中通过不同的TokenType来进行语法解析。

#### 2.2.1 算术运算符

| 字符串 | TokenType    | 说明                       |
| ------ | ------------ | -------------------------- |
| -      | MINUS        | 一元运算负号；二元运算减法 |
| +      | PLUS         | 二元运算加法               |
| *      | MULTI        | 二元运算乘法               |
| /      | DIV          | 二元运算除法               |
| %      | MOD          | 二元运算取余               |
| =      | ASSIGN       | 二元运算赋值               |
| -=     | MINUS_ASSIGN | 二元运算减法并赋值         |
| +=     | PLUS_ASSIGN  | 二元运算加法并赋值         |
| *=     | MULTI_ASSIGN | 二元运算乘法并赋值         |
| /=     | DIV_ASSIGN   | 二元运算除法并赋值         |
| %=     | MOD_ASSIGM   | 二元运算取余并赋值         |

* 在Eagle中，赋值运算是有返回值的，返回值为赋值后的结果，所以将赋值运算符归为算数运算符

#### 2.2.2 关系运算符

| 字符串 | TokenType     | 说明                             |
| ------ | ------------- | -------------------------------- |
| ==     | EQUAL         | 检查左右操作数是否相等           |
| !=     | NOT_EQUAL     | 检查左右操作数是否不相等         |
| <      | LESS          | 检查左操作数是否小于右操作数     |
| <=     | LESS_EQUAL    | 检查左操作数是否小于等于右操作数 |
| >      | GREATER       | 检查左操作数是否大于右操作数     |
| >=     | GREATER_EQUAL | 检查左操作数是否大于等于右操作数 |

#### 2.2.3 字面量标识符

| TokenType  | 说明                         |
| ---------- | ---------------------------- |
| IDENTIFIER | 标识符，如 a，abc            |
| NUMBER     | 数字，如 1，1.2              |
| STRING     | 字符串，如 "abcd"            |
| BOOLEAN    | 布尔值，如 true，false       |
| NIL        | 空，如nil                    |
| LIST       | 列表，如 [1, true, "123"]    |
| DICT       | 字典，如 {1: 2, true: "abc"} |
| TUPLE      | 元组，如 (a, function, 1)    |

* 字面量标识符，部分字面量标识符可以在Lexer词法解析时直接解析出来，部分字面量标识符需要在Parser语法解析时通过分析嵌套表达式来解析
  * Lexer直接解析：IDENTIFIER，NUMBER，STRING，BOOLEAN
  * Parser嵌套解析：LIST，DICT，TUPLE

#### 2.2.4 关键字

| 字符串   | TokenType | 说明                |
| -------- | --------- | ------------------- |
| var      | VAR       | var变量声明关键字   |
| and      | AND       | 二元逻辑运算与      |
| or       | OR        | 二元逻辑运算或      |
| not      | NOT       | 一元逻辑运算非      |
| nil      | NIL       | 基本类型，空关键字  |
| for      | FOR       | for循环关键字       |
| while    | WHILE     | while循环关键字     |
| break    | BREAK     | break跳出循环       |
| continue | CONTINUE  | continue持续循环    |
| if       | IF        | if条件关键字        |
| else     | ELSE      | else条件关键字      |
| switch   | SWITCH    | switch条件关键字    |
| case     | CASE      | case条件关键字      |
| default  | DEFAULT   | default条件关键字   |
| stream   | STREAM    | stream流处理关键字  |
| def      | DEF       | def函数声明关键字   |
| class    | CLASS     | class类声明关键字   |
| super    | SUPER     | super类操作关键字   |
| this     | THIS      | this类操作关键字    |
| extends  | EXTENDS   | extends类继承关键字 |
| print    | PRINT     | print输出关键字     |
| return   | RETURN    | return函数返回      |

#### 2.2.5 基本符号

| 字符串 | TokenType        | 说明     |
| ------ | ---------------- | -------- |
| ,      | COMMA            | 逗号     |
| .      | DOT              | 点       |
| ;      | SEMICOLON        | 分号     |
| ?      | QUESTION_MARK    | 问好     |
| !      | EXCLAMATION_MARK | 感叹号   |
| (      | LEFT_PAREN       | 左小括号 |
| )      | RIGHT_PAREN      | 右小括号 |
| [      | LEFT_BRACKET     | 左中括号 |
| ]      | RIGHT_BRACKET    | 右中括号 |
| {      | LEFT_BRACE       | 左大括号 |
| }      | RIGHT_BRACE      | 右大括号 |
| ->     | RIGHT_ARROW      | 右箭头   |

#### 2.2.6 辅助标识符

| TokenType | 说明                                             |
| --------- | ------------------------------------------------ |
| END       | 在词法解析结束后，在末尾添加END标识最后一个Token |

### 2.3 Lexer设计

Eagle解释器中的Lexer词法解析，采用手动构造的方式来实现，没有依赖第三方支持。

#### 2.3.1 处理逻辑

* Eagle解释器主要将输入分为五类进行处理
  1. 可直接识别的字符：包括算数运算符，关系运算符，基本符号等
  2. 数字：需要按照NUMBER对应的格式，识别完整的数字，并初始化数字对应的BigFloat Object对象，存储到Token的literal字段中
  3. 字符串：在识别到`"`符号后，进入字符串的解析，需要将完整的字符串初始化为对应的String Object对象中，存储到Token的literal字段中
  4. 关键字/标识符：关键字和标识符有一样的解析逻辑，需要在解析之后判断该标识符是否为Eagle的关键字，并赋予对应的TokenType
  5. 特殊字符：
     * `\n, \t, \r`和空格，符合Eagle字符规范的输入会进行忽略处理，其中`\n`会让line的计数增加
     * 不符合Eagle字符规范的输入，会在Lexer语法解析的时候**报错**

#### 2.3.2 处理过程

<img src="./imgs/Lexer处理过程.png" alt="Lexer处理过程" style="zoom:20%;" />

## 3. 语法设计

### 3.1 EBNF设计

#### 3.1.1 Program入口 EBNF设计

```cpp
program ::= (declaration|statement)* ;
```

#### 3.1.2 Statement EBNF设计

* 声明

```cpp
declaration :: = class-declaration |
				 function-declaration |
				 variable-declaration ;

class-declaration ::= "class" identifier ("extends" identifier)?
					  "{" (variable-declaration|function-declaration)* "}" ;

function-declaration ::= "def" identifier "(" parameters? ")" block-statement ;
parameters ::= identifier ("," identifier)* ;

variable-declaration ::= "var" identifier ("=" expression)? ";" ;
```

* 语句

```cpp
statement ::= if-statement |
			  for-statement |
              while-statement |
              expression-statement |
			  print-statement |
			  return-statement |
			  break-statement |
			  continue-statement |
			  block-statement |
			  empty-statement;

if-statement ::= "if" "(" condition ")" statement
				 ("else" statement)? ;

for-statement ::= "for" "(" 
				  (variable-declaration | expression-statement | ";")?
				  expression? ";"
				  expression? 
				  ")"
				  statement ;

while-statement ::= "while" "(" expression ")" statement ;

expression-statement ::= expression ";" ;

print-statement ::= "print" expression ";" ;

return-statement ::= "return" expression ";" ;

break-statement ::= "break" ";" ;

continue-statement ::= "continue" ";" ;

block-statement ::= "{" (declaration|statement)* "}" ;

empty-statement ::= ";" ;
```

#### 3.1.3 Expression EBNF设计

```cpp
// Expression
expression ::= assignment_expression ;

assignment_expression ::= call ("="|"+" "="|"-" "="|"*" "="|"/" "="|"%" "=") 		
						  assignment_expression | ternary_if_else ;

ternary_if_else ::= logic_or ("?" ternary_if_else ":" ternary_if_else)? ;

logic_or ::= logic_and ("or" logic_and)* ;

logic_and ::= comparison ("and" comparison)* ;

comparison ::= term (("=="|"!="|">"|">="|"<"|"<=") term)* ;

term ::= factor (("-"|"+") factor)* ;

factor ::= unary (("*"|"/"|"%") unary)* ;

unary ::= ("!"|"-"|"not") unary | call ;

call ::= primary ("." identifier | ("[" expression "]")+ | "(" arguments? ")")* ;

primary ::= "true" | "false" | "nil" | "this" | "super" "." identifier |
			number | string | identifier | "(" expression ")" |
			"[" list_elements? "]" | "{" dict_elements? "}" | "(" tuple_elements? ")" |
			stream_expression | switch_expression | lambda_expression ;
```

#### 3.1.4 基本单元 EBNF设计

```cpp
list_elements ::= expression ("," expression)* ;

dict_elements ::= expression ":" expression ("," expression ":" expression)* ;

tuple_elements ::= (expression ",")? | expression ("," expression)+ ;

stream_expression ::= "stream" "(" expression ")" ("." identifier "(" call? ")")* ;

switch_expression ::= "switch" "(" expression ")" "{"
					  ("case" expression ":" expression ",")*
					  "default" ":" expression "}" ;

lambda_expression ::= "(" parameters? ")" "->" (expression | block-statement) ;

arguments ::= expression ("," expression) ;

number ::= digit+ ("." digit+)? ;

digit ::= "0"..."9" ;

string ::= "\"" <any char except \">* "\""
  
identifier ::= alpha (alpha|digit)* ;

alpha ::= "a"..."z"|"A"..."Z"|"_" ;
```

### 3.2 AST抽象语法树设计

抽象语法树（AST）是通过Parser语法解析，将Lexer词法解析的结果构造为Eagle对应的语言成分，在Eagle中主要分为Expression和Statement两类

在Eagle的AST抽象语法树中，入口为Program，是一个List\<Statement\>，里面包含多个Statement AST节点。在Expression AST节点和Statement AST节点中，它们的节点组成主要为Token，Expression，Statement，

#### 3.2.1 Expression AST节点设计

```cpp
Assign 				: Token name, Token operator, Expr value
Ternary 			: Expr condition, Expr then_expr, Expr else_expr
Compare 			: Expr first, List<Pair<Token, Expr>> others
Binary 				: Expr left, Token operator, Expr right
Unary 				: Token operator, Expr expr
Call 				: Expr callee, List<Expr> arguments, int line
Literal 			: Object value
Variable 			: Token name
Stream 				: Expr expr, List<Pair<Token, Expr>> operations
Switch 				: Expr expr, List<Pair<Expr, Expr>> case_results, Expr default_result
Lambda				: List<Token> params, Stmt body
InstanceSet 		: Expr object, Token name, Token operator, Expr value
InstanceGet 		: Expr object, Token name
ContainerSet		: Expr container, Expr subscript, Token operator, Expr value
ContainerGet		: Expr container, Expr subscript
This 				: Token keyword
Super 				: Token keyword, Token method
Sequence 			: Token type, List<Expr> elements
Associative 		: Token type, List<Pair<Expr, Expr>> elements
```

#### 3.2.2 Statement AST节点设计

```cpp
Class 		: Token name, Variable super_class, List<Var> members, List<Function> methods
Function 	: Token name, List<Token> params, List<Stmt> body
Var 		: Token name, Expr initializer
If 			: Expr condition, Stmt then_branch, Stmt else_branch
While 		: Expr condition, Stmt body
For       	: Stmt initializer, Expr condition, Stmt increment, Stmt body 
Expression	: Expr expression
Print 		: Expr print_value
Return 		: Expr return_value, int line
Break 		: Token keyword
Continue 	: Token keyword
Block 		: List<Stmt> statements
Empty     	:
```

#### 3.2.3 Eagle AST样例说明

##### InstanceSet AST样例说明

```java
class A {
	var b = 1;
}
A().b += 2;
```

<img src="./imgs/InstanceSet.png" alt="InstanceSet" style="zoom:18%;" />

##### ContainerSet AST样例说明

```java
var b = {1: 2, 3: 4};
b[3] = "4";
```

<img src="./imgs/ContainerSet.png" alt="ContainerSet" style="zoom:18%;" />

##### Function AST样例说明

```cpp
def func(a, b) {
	var c = a;
	return c + b;
}
var b = func(1, 2);
```

<img src="./imgs/Function.png" alt="Function" style="zoom:18%;" />

##### For AST样例说明

```java
for (var a = 1; a < 10; a += 1) {
	print a;
}
```

<img src="./imgs/For.png" alt="For" style="zoom:18%;" />



## 4. 语义说明

### 4.1 存储域

Store是Eagle的存储域，描述的是程序运行时的堆内存的使用。Location用于描述存储域中存储数据的位置，使用Location和辅助函数fetch可以查询存储域中的值，使用Location和update函数可以更新存储域中的值，allocate，deallocate等辅助函数更新存储域的使用情况。

* 语义函数

```
Store = Location → (stored Storable + undefined + unused)
```

* 辅助函数

```
empty_store: Store
allocate: Store → Store × Location
deallocate: Store × Location → Store
update: Store × Location × Storable → Store
fetch: Store × Location → Storable
```

* 辅助函数符号表示

```
empty_store = λloc.unused
allocate(sto) = let loc = any_unused_location(sto) in (sto[loc → undefined], loc)
deallocate(sto, loc) = sto[loc → unused]
update(sto, loc, stble) = sto[loc → stored stble]
fetch(sto, loc) = let stored_value (stored stble) = stble
				  	  stored_value (undefined) = fail
				  	  stored_value (unused) = fail
				  	  in stored_value (sto(loc))
```

### 4.2 环境域

标识环境域Environ，标识符域Identifier，可绑定体域Bindable，环境特性的映射关系如下：

* 语义函数

```
Environ = Identifier → (bound Bindable + unbound)
```

* 辅助函数

```
empty_environ: Environ
bind: Identifier × Bindable → Environ
overlay: Environ × Environ → Environ
find: Environ × Identifier → Bindable
```

* 辅助函数符号表示

```
empty-environ = λI.unbound
bind(I, bdble) = λI'. if I' = I then bound bdble else unbound
overlay(env', env) = λI. if env'(I) != unbound then env'(d) else env(d)
find(env, I) = 
	let bound_value(bound bdble) = bdble
		bound_value(unbound) = fail
	in bound_value(env(I))
```

### 4.3 原始域表达式

Primary原始域中包含：真值域Boolean，空值域Nil，字符串域String，数值域Number，容器域Container，标识符域Identifier。

* 语义函数

```
Primary = Value → (Boolean + Nil + String + Number + Container + Identifier)
```

* 辅助函数

```
fetch_variable: Store × Identifier → Value
instance_get: Identifier × Bindable × Identifier → Value
container_get: Environ × Identifier × Expression → Value
```

* 辅助函数符号表示

```
fetch_variable(sto, variable loc) = fetch(sto, loc)
instance_get(instance, env, member) = 
	let instance_sto = find(env, instance) in
	fetch_variable(instance_sto, member)
container_get(container, env, index) = 
	let container_sto = find(env, container) in
	fetch_variable(container_env, index)
```

### 4.4 表达式

使用Expression来表示表达式域，表达式主要可以分为取值表达式，赋值表达式，算术表达式，关系表达式和逻辑表达式。

* 语义函数

```
evaluate: Expression → (Environ → Store → Value)
```

* 辅助函数

```
update_variable: Store × Identifier × Value → Store
// 赋值表达式
instance_set: Identifier × Bindable × Identifier × Value → Environ
container_set: Environ × Identifier × Expression × Value → Environ
// 算术表达式
plus: Value × Value → Value
minus: Number × Number → Number
multi: Number × Number → Number
div: Number × Number → Number
mod: Number × Number → Number
// 关系表达式
equal: Value × Value → Boolean
not_equal: Value × Value → Boolean
less: Value × Value → Boolean
less_equal: Value × Value → Boolean
greater: Value × Value → Boolean
greater_equal: Value × Value → Boolean
// 逻辑表达式
not: Boolean → Boolean
and: Boolean × Boolean → Boolean
or: Boolean × Boolean → Boolean
```

* 辅助函数符号表示

```
update_variable(sto, variable loc, stble) = update(sto, loc, stble)
// 赋值表达式
instance_set(instance, env, member, sto, value) = 
	let instance_var = intance_get(instance, env, member) in
	update_variable(sto, instance_var, value)
container_set(container, env, index, sto, value) =
	let container_var = container_get(container, env, index) in
	update_variable(sto, container_var, value)
// 算术表达式
plus(left, right) = let plus_value (Number left, Number right) = left + right
						plus_value (String left, String right) = left + right // 字符串拼接
						plus_value (Value left, Value right) = fail
						in plus_value (left, right)
minus(left, right) = left - right
multi(left, righ) = left * right
divide(left, right) = left / right
mod(left, right) = left % right
// 关系表达式
equal(left, right) = 
	if left == right
	then true
	else false
not_equal(left, right) = 
	if left != right
	then true
	else false
less(left, right) =
	let less_value (Number left, Number right) = 
			if left < right then true else false
		less_value (String left, String right) = 
			if left < right then true else false // 比较字符串字典序
		less_value (Value left, Value right) = fail
		in less_value (left, right)
less_equal(left, right) =
	let less_equal_value (Number left, Number right) = 
			if left <= right then true else false
		less_equal_value (String left, String right) = 
			if left <= right then true else false // 比较字符串字典序
		less_equal_value (Value left, Value right) = fail
		in less_equal_value (left, right)
greater(left, right) =
	let greater_value (Number left, Number right) = 
			if left > right then true else false
		greater_value (String left, String right) = 
			if left > right then true else false // 比较字符串字典序
		greater_value (Value left, Value right) = fail
		in greater_value (left, right)
greater_equal(left, right) =
	let greater_equal_value (Number left, Number right) = 
			if left >= right then true else false
		greater_equal_value (String left, String right) = 
			if left >= right then true else false // 比较字符串字典序
		greater_equal_value (Value left, Value right) = fail
		in greater_value (left, right)
// 逻辑表达式
not(value) = !value
and(left, right) =
	if left == true && right == true
	then true
	else false
or(left, right) = 
	if left == true || right == true
	then true
	else false
```

* 语义方程

```
// 取值表达式
evaluate [I.member] env sto =
	intance_get(I, env, member)
evaluate [I[E]] env sto = 
	let index = evaluate E env sto in
	container_get(I, env, index)
// 赋值表达式
evaluate [I = E] env sto =
	let val = evaluate E env sto in
	update_variable(sto, I, val)
evaluate [I.member = E] env sto =
	let val = evaluate E env sto in
	instance_set(I, env, member, sto, val)
evaluate [I[E1] = E2] env sto = 
	let index = evaluate E1 env sto in
	let val = evaluate E2 env sto in
	container_set(I, env, index, sto, val)
// 算术表达式
evaluate [E1 + E2] env sto =
	let left = evaluate E1 env sto in
	let right = evaluate E2 env sto in
	plus(left, right)
evaluate [E1 - E2] env sto =
	let left = evaluate E1 env sto in
	let right = evaluate E2 env sto in
	minus(left, right)
evaluate [E1 * E2] env sto =
	let left = evaluate E1 env sto in
	let right = evaluate E2 env sto in
	multi(left, right)
evaluate [E1 / E2] env sto =
	let left = evaluate E1 env sto in
	let right = evaluate E2 env sto in
	divide(left, right)
evaluate [E1 % E2] env sto =
	let left = evaluate E1 env sto in
	let right = evaluate E2 env sto in
	mod(left, right)
// 关系表达式
evaluate [E1 == E2] env sto =
	let left = evaluate E1 env sto in
	let right = evaluate E2 env sto in
	equal(left, right)
evaluate [E1 != E2] env sto =
	let left = evaluate E1 env sto in
	let right = evaluate E2 env sto in
	not_equal(left, right)
evaluate [E1 < E2] env sto =
	let left = evaluate E1 env sto in
	let right = evaluate E2 env sto in
	less(left, right)
evaluate [E1 <= E2] env sto =
	let left = evaluate E1 env sto in
	let right = evaluate E2 env sto in
	less_equal(left, right)
evaluate [E1 > E2] env sto =
	let left = evaluate E1 env sto in
	let right = evaluate E2 env sto in
	greater(left, right)
evaluate [E1 >= E2] env sto =
	let left = evaluate E1 env sto in
	let right = evaluate E2 env sto in
	greater_equal(left, right)
// 逻辑表达式
evaluate [not E] env sto =
	let value = evaluate E env sto in
	not(value)
evaluate [E1 and E2] env sto =
	let left = evaluate E1 env sto in
	let right = evaluate E2 env sto in
	and(left, right)
evaluate [E1 or E2] env sto =
	let left = evaluate E1 env sto in
	let right = evaluate E2 env sto in
	or(left, right)
// switch-case表达式
evaluate [switch(E) case E1: V1, case E2: V2, default V3] env sto =
	let condition = evaluate E env sto in
	let case_1 = evaluate E1 env sto in
	if condition == case_1
	then evaluate V1
	else
	let case_2 = evaluate E2 env sto in
	if condition == case_2
	then evaulate V2
	else
	evaluate V3
// 其他表达式
evaluate [(E)] env sto = evaluate E env sto
```

### 4.5 语句

* 使用Statement来表示语句域，语句主要可以分为条件语句，循环语句，表达式语句，输出语句，空语句。

* 语义函数

```
execute: Statement → (Environ → Store → Store)
```

* 语义方程

```
execute [if C then S1 else S2] env sto = 
	if evaluate C env sto = Boolean true
	then execute S1 env sto
	else execute S2 env sto
execute [for (S1; C; S2) S3] env sto =
	execute S1 env sto
	let execute_for env sto = 
		if evaluate C env sto = Boolean true
		then execute_for env (execute S2 env sto (execute S3 env sto))
		else sto
	in
	execute_for
execute [while C do S] env sto = 
	let execute_while env sto =
		if evaluate C env sto = Boolean true
		then execute_while env (execute S env sto)
		else sto
	in
	execute_while
execute [E] env sto = evaluate E env sto
execute [print E] env sto =
	let output = evaluate E env sto in
	print output
execute [] env sto = sto
```

### 4.6 声明

* 在Eagle中声明分为变量声明，函数声明和类的声明

#### 变量

* 语义方程

```
execute [var V = E] env sto =
	let var = identifier V env sto in
	let val = evaluate E env sto in
	update_variable(allocate(sto), var, val)
```

#### 函数

在Eagle关于函数，分为函数的声明与形式参数Formal Parameters，以及函数的调用与实际参数Actual Parameters。

* 函数抽象

```
Function = Argument → Value
Function = Argument → Store → Value
```

* 辅助函数

```
bind_parameter: Formal_Parameter → (Argument → Environ)
give_argument: Actual_Parameter → (Environ → Argument)
```

* 辅助函数符号表示

```
bind_parameter(I, arg) = bind(I, arg)
give_argument(E) env sto = let arg = evluate E env sto
```

* 语义方程

```
// 函数声明
execute [def I(FP) S] env =
	let func arg =
		let parenv = bind_parameter(I, FP) in
		evaluate S(overlay(parenv, env))
	in
	(bind(I, def func))
// 函数调用
evaluate [I(AP)] env =
	let function func = find(env, I) in
	let arg = give_argument(AP) env in
	func arg
```

#### 类

* 类抽象

 ```
 Class = Class_Declaration → (Environ → Store → Environ × Store)
 ```

* 语义方程

```
execute [class I1 extends I2 VD FD] env sto =
	let super_class = find(env, I2) in
	let class sto' = 
		let env = overlay(bind(I, class class), overlay(super_class, env)) in
		let variables = execute VD env sto
		let functions = execute FD env sto
		in
		(bind(I1, class class), sto')
```

## 5. 对标语言差异



## 6. 实现细节

### 6.1 总述

Eagle语言的解释器使用c++17实现，可以运行在windows，linux，macOS等常见操作系统上。

解释器的整体实现思路为使用访问者模式（Visitor Pattern），在通过词法分析和语法分析得到AST后，从根节点开始递归地访问AST的每一个节点，并针对不同的AST节点类型执行不同的动作，并最终完成源代码的解释执行。若在执行过程中发生错误，则抛出`EagleRuntimeError`（运行时错误）异常到解释器的最顶层，解释器捕捉到该异常后打印错误信息并停止解释执行。

使用面向对象的编程范式完成代码的具体实现，将模块功能封装在相应的类中。为了方便管理内存，使用c++17的智能指针`shared_ptr`管理堆内存资源。

### 6.2 Object类

#### 6.2.1 使用Object类的原因

出于以下两个方面的原因，程序中使用自定义的Object类作为公共父类：

+ 在访问AST节点时，需要使用RTTI（Run Time Type Identification，运行时类型识别）来确定子节点的类型，从而确定处理逻辑；
+ Eagle语言的值Value没有具体的类型，可以是字符串、数值、列表等等，为了方便实现，将Value定义为公共父类的指针，并在解释执行时通过RTTI确定子节点的类型并选择相应的处理方法。

出于以上两点考虑，定义Object类作为所有AST节点类型和Eagle所有值类型的公共父类，其继承关系如下图所示

<img src="./imgs/Object继承关系.png" alt="Object继承关系" style="zoom:80%;" />

其中：

+ 上半部分为AST节点类型，在3.2节中已介绍
+ 下半部分为Eagle中的值类型，将在后续章节中介绍，包括：
	+ 空值 Null
	+ 布尔值 Boolean
	+ 数值 Number
	+ 具有内置方法的类型 BuiltInClass，包括：
		+ 容器类型 Container，包含：列表 List，元组 Tuple，字典 Dict
		+ 字符串类型 String
	+ 流类型 Stream
	+ 对象类型（即类实例）Instance
	+ 可调用类型 Callable，包括：
		+ 用户自定义类 EagleClass
		+ 用户自定义函数 EagleFunction
		+ 匿名函数（lambda表达式）Lambda
		+ 内置函数 BuiltInFunction
		+ 其他

#### 6.2.2 Object类的方法

考虑到Eagle的各种值类型具有一些公共的行为，参考java语言Object类的方法，设计Object类具有以下四个可继承的方法：

1. `toString`
	+ 含义：返回该Eagle值的字符串表示形式
	+ Object类行为：返回字符串`"<object at address>"`，其中address为该值的内存地址
2. `equals(Object obj)`
	+ 含义：返回该Eagle值是否与另一个Eagle值`obj`相等
	+ Object类行为：返回自身的内存地址是否与另一个Eagle值的内存地址相等
3. `hashcode`
	+ 含义：返回该Eagle值的hash值
	+ Object类行为：返回该值的内存地址
4. `isTruthy`
	+ 含义：返回该Eagle值作为布尔值进行判断时，是否为真
	+ Object类行为：返回true

### 6.3 内置类型

#### 6.3.1 Null

Null为Eagle中的空值，当用户显式地将某个变量赋值为`nil`时，该变量的值为Null；或当某个函数没有通过return语句返回时，该函数的返回值为Null.

Null不具有任何成员变量，仅重写了父类Object的四个方法：

1. `toString`：返回字符串`"Nil"`
2. `equals(Object obj)`：所有Null类型的值均相等，因此该方法返回`obj`的类型是否为Null
3. `hashcode`：返回0
4. `isTruthy`：返回false

#### 6.3.2 Boolean

Boolean为Eagle中的布尔值，当用户将某个变量显式地赋值为`true`，`false`，或赋值为某个逻辑表达式的结果时，该变量的值类型为Boolean

Boolean具有类型为bool的成员变量value，表示该Boolean值的真假，并重写了父类Object的四个方法：

1. `toString`：若value为真，则返回字符串`"true"`，否则返回字符串`"false"`
2. `equals(Object obj)`：仅当`obj`的类型也为Boolean，且二者的value值相等时，返回true，否则返回false
3. `hashcode`：返回value的hash值（使用c++内置的hash函数）
4. `isTruthy`：返回value

#### 6.3.3 Number

Number为Eagle中的数值，包括高精度整数和高精度浮点数，并支持任意精度的结果精确的数值基本运算（加减乘除模）

Number的实现基于开源的[BigFloat](https://github.com/Mariotti94/BigFloat)，其包含以下成员变量：

+ sign：表示Number的符号
+ number：类型为`deque<char>`，倒序存储了Number的十进制表示的每一位的数字
+ decimals：类型为`int`，表示Number小数点的位置

Number实现了数值基本运算方法、比较方法、与int、double、string类型的相互转换方法等，并重写了父类Object的四个方法：

1. `toString`：返回数值的字符串表示形式，如-1.23返回`"-1.23"`
2. `equals(Object obj)`：若`obj`也为Number类型，则从最高位逐位进行比较，否则返回false
3. `hashcode`：返回数值的字符串表示的hash值（使用c++内置的hash函数）
4. `isTruthy`：若数值不为0，则返回true，否则返回false

#### 6.3.4 Callable

Callable为Eagle中的可调用类型，包括EagleClass, EagleFunction, Lambda等，这些对象具有相同的公共行为，即通过`()`运算符、传入参数并进行函数调用。可将该行为抽象为如下的抽象方法：

```c++
Object call(vector<Object> arguments)
```

所有Callable的子类均需要实现该方法。

该类型与AST中Call节点的处理紧密相关，具体而言，需要判断Call节点的callee进行evaluate后的结果，是否为Callable类型。若否，则表示该callee不可被调用，此时需要报错；若是，则调用该Callable类型的call方法即可。

#### 6.3.5 BuiltInClass

BuiltInClass表示Eagle中具有内置方法的类型，包含String，Container等。这些类型的值具有相同的抽象行为，即可以通过形如`instance.method(params)`对内置方法进行调用，例如：

+ 若`s`为字符串类型，则可以通过`s.size()`获得`s`的长度；
+ 若`l`为列表类型，v为任意Eagle值类型，则可以通过`l.append(v)`将`v`添加到`l`的末端

对于上述行为的解释执行，实现为一个callback过程，具体而言，分为两个阶段：

1. 处理InstanceGet节点：通过`method`的名称，获取对应的内置方法的函数指针`method_f`，将其与该Eagle值的指针`instance`存储到`BuiltInClassCall`对象中并返回给上层节点，其中：
	+ `method_f`为静态函数指针类型，其返回值类型为Object，第一个参数类型为BuiltInClass，代表需要执行内置方法的Eagle值；第二个参数类型为`vector<Object>`，即上层Call节点中的参数；
	+ `BuiltInClassCall`是一个`Callable`类型；
2. 处理Call节点：调用上一步获得的`BuiltInClassCall`对象的`call`方法，通过调用`method_f(instance, params)`来执行对应的内置方法，其中`params`为Call节点中的参数，类型为`vector<Object>`。

分析上述执行过程，可以发现BuiltInClass的抽象行为为，通过方法名获取对应的内置方法的函数指针，将该抽象行为定义为抽象方法`GetMethod(string name)`，其子类需要实现该方法。

#### 6.3.6 Container

Container表示Eagle中的容器，包含List, Tuple和Dict，是Eagle中的一类BuiltInClass。

Eagle中，Container的抽象行为包括：

1. `Object get(Object subscript)`：使用`[]`运算符，通过下标获取容器值；
2. `void set(Object subscript, Object value)`：使用`[]`运算符，通过下标设置容器值；
3. `Number size()`：获取容器大小；
4. `vector<Object> iterator()`：获取容器的迭代元素序列，从而对容器进行迭代

下面介绍List, Tuple, Dict三种容器的行为、实现以及具有的内置方法。

##### List



##### Tuple



##### Dict



#### 6.3.7 String



#### 6.3.8 Stream



### 6.4 内置函数



### 6.5 类与实例



### 6.6 Return, Break, Continue实现



### 6.7 函数 / lambda



### 6.8 EagleShell



### 6.9 函数调用栈的层数控制



## 7. 验证与测试

1. 基本功能
   1. 类使用
   2. 容器使用
   3. ...
2. 错误示例