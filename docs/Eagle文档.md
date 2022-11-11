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

// 表达式
expression ::= assignment_expression ;

assignment_expression ::= (call ".")? identifier ("[" assignment_expression "]")* 
													("="|"+" "="|"-" "="|"*" "="|"/" "="|"%" "=") assignment_expression |
													ternary_if_else ;

ternary_if_else ::= logic_or ("?" ternary_if_else ":" ternary_if_else)? ;
logic_or ::= logic_and ("or" logic_and)* ;
logic_and ::= equality ("and" equality)* ;
equality ::= comparison (("!="|"==") comparison)* ;
comparison ::= term ((">"|">="|"<"|"<=") term)* ;						 
term ::= factor (("-"|"+") factor)* ;
factor ::= unary (("*"|"/"|"%") unary)* ;
unary ::= ("!"|"-"|"not") unary | call ;
call ::= subscript ( "(" arguments? ")" | "." identifier)* ;
subscript ::= primary ("[" expression "]")* ;
primary ::= "true" | "false" | "nil" | "this" | "super" "." identifier |
						number | string | identifier | "(" expression ")" |
						"[" list_elements? "]" | "{" dict_elements? "}" | "(" tuple_elements? ")" |
						stream_expression | switch_expression ;

list_elements ::= expression ("," expression)* ;
dict_elements ::= expression ":" expression ("," expression ":" expression)* ;
typle_elements ::= (expression ",")? | expression ("," expression)* ;

stream_expression ::= "stream" "(" expression ")" ("." identifier "(" identifier? ")")* ;
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
MULTI // *
DIV // /
MOD // %
MINUS_EQUAL // -=
PLUS_EQUAL // +=
MULTI_EQUAL // *=
DIV_EQUAL // /=
MOD_EQUAL // %=
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
 BOOLEAN
 LIST
 DICT
 TUPLE
 ```

### 关键字

 ```
 VAR // var
 
 AND // and
 OR // or
 NOT // not
 
 TRUE-->BOOLEAN // true
 FALSE-->BOOLEAN // false
 NIL // nil
 
 FOR // for
 WHILE // while
 BREAK // break
 CONTINUE // continue
 
 IF // if
 ELSE // else
 
 SWITCH // switch
 CASE // case
 DEFAULT // default
 
 STREAM // stream
 
 DEF // def
 
 CLASS // class
 SUPER // super
 THIS // this
 EXTENDS // extends
 
 PRINT // print
 RETURN // return
 
 END // 辅助终止符
 ```

### 其他符号

```
ASSIGN // =
COMMA // ,
DOT // .
COLON // :
SEMICOLON // ;
QUESTION_MARK // ?
EXCLAMATION_MARK // !
LEFT_PAREN // (
RIGHT_PAREN // )
LEFT_BRACKET // [
RIGHT_BRACKET // ]
LEFT_BRACE // {
RIGHT_BRACE // }
```

## 其他

* 注释："//" + ...
* list添加/删除元素：push，pop
* dict添加/删除元素：insert，remove
* steam--基本类型
  * 非终结方法：map/filter/limit/sorted，返回值为stream
    * map：将元素进行映射
    * filter：通过check的方法，对元素进行过滤
    * limit：取出前k个元素
    * sorted：对元素进行排序
  * 终结方法：to_list/to_dict/to_tuple/for_each/count
    * to_list/to_tuple：将流元素转换为对应的可迭代类型，返回值为list/tuple
    * to_dict：对二元流元素转换为对应的可迭代类型，返回值为dict
    * for_each：对每个元素都执行该方法，返回值为void
    * count：统计流元素个数，返回值为int
* 类初始化init函数内置
