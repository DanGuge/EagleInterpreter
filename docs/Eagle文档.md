# Eagle文档

## ENBF

```
program ::= (declaration|statement)* ;

// 声明
declaration :: = class-declaration |
								 function-declaration |
								 variable-declaration ;

class-declaration ::= "class" identifier ("extends" identifier)?
											"{" (variable-declaration|function-declaration)* "}" ;

function-declaration ::= "def" identifier "(" parameters? ")" block-statement ;
parameters ::= identifier ("," identifier)* ;

variable-declaration ::= "var" identifier ("=" expression)? ";" ;

// 语句
statement ::= if-statement |
							for-statement |
							while-statement |
							expression-statement |
							print-statement |
							return-statement |
							break-statement |
							continue-statement |
							block-statement ;

if-statement ::= "if" "(" condition ")" statement
								 ("else" statement)? ;

for-statement ::= "for" "(" 
									(variable-declaration | expression-statement| ";")?
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

// 表达式
expression ::= assignment |
							 logic_expression |
							 number_expression |
							 switch_expression ;

assignment ::= (call ".")? identifier ("[" expression "]")* "=" expression ;

logic_expression ::= logic_and ("or" logic_and)* ;
logic_and ::= comparison ("and" comparison)* ;
comparison ""= expression (!="|"=="|">"|">="|"<"|"<=" expression)* ;
										 
number_expression ::= factor (("-"|"+") factor)* ;
factor ::= unary (("*"|"/"|"%") unary)* ;
unary ::= ("!"|"-") unary | call ;
call ::= subscript ( "(" arguments? ")" | "." identifier)* ;
subscript ::= primary ("[" expression "]")* ;
primary ::= "true" | "false" | "null" | "this" | "super" "." identifier |
						number | string | identifier | "(" expression ")" |
						"[" elements? "]" | "{" key_values? "}" ;
elements ::= expression ("," expression)* ;
key_values ::= expression ":" expression 
							 ("," expression ":" expression)* ;

switch_expression ::= "switch" "(" expression ")" "{"
											("case" expression ":" expression ",")*
											"default" ":" expression "}" ;

arguments ::= expression ("," expression) ;

number ::= digit+ ("." digit+)? ;
digit ::= "0"..."9" ;
string ::= "\"" <any char except \">* "\""
identifier ::= alpha (alpha|digit)* ;
alpha ::= "a"..."z"|"A"..."Z"|"_" ;
```



## Token

### 算术运算符

```
MINUS // -
PLUS // +
MULT // *
DIV // /
MOD // %
```

### 关系运算符

```
EQUAL // ==
NOT_EQUAL // !=
LESS // <
LESS_EQUAL // <=
GREATER // >
GREATER_EQUAL // >=
```

### 基本类型

 ```
 IDENTIFIER
 NUMBER
 STRING
 LIST
 DICT
 ```

### 关键字

 ```
 VAR // var
 
 AND // and
 OR // or
 
 TRUE // true
 FALSE // false
 NULL // null
 
 FOR // for
 WHILE // while
 BREAK // break
 CONTINUE // continue
 
 IF // if
 ELSE // else
 
 SWITCH // switch
 CASE // case
 DEFAULT // default
 
 DEF // def
 
 CLASS // class
 SUPER // super
 THIS // this
 EXTENDS // extends
 
 PRINT // print
 RETURN // return
 
 EOF // 辅助终止符
 ```

### 其他符号

```
ASSIGN // =
COMMA // ,
DOT // .
COLON // :
SEMICOLON // ;
LEFT_PAREN // (
RIGHT_PAREN // )
LEFT_BRACKET // [
RIGHT_BRACKET // ]
LEFT_BRACE // {
RIGHT_BRACE // }
```

## 其他

* 注释："//" + ...

* list添加/删除元素：push，pop？
* map添加/删除元素：insert？
* filter/map/reduce：当内置方法实现？
* 类初始化init函数内置
