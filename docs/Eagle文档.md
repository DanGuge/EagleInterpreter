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
							block-statement  |
                            empty-statement ;

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

// 表达式
expression ::= assignment_expression ;

assignment_expression ::= call ("="|"+" "="|"-" "="|"*" "="|"/" "="|"%" "=") assignment_expression |
						  ternary_if_else ;

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



## Token

### 算术运算符

```
MINUS // -
PLUS // +
MULTI // *
DIV // /
MOD // %
MINUS_ASSIGN // -=
PLUS_ASSIGN // +=
MULTI_ASSIGN // *=
DIV_ASSIGN // /=
MOD_ASSIGN // %=
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
  * [ ] 非终结方法：map/filter/limit/sorted，返回值为stream
    * [x] one-to-one methods：
      * [x] filter(func)：使用func方法，对元素进行过滤
      * [x] map(func)：使用func方法，对元素进行映射
    * [ ] many-to-many methods：
      * [ ] sorted(func)：若func不为空，则使用func进行比较，并升序排序元素；否则若流元素均为同一类可比较类型，则按照对元素进行排序（需要先实现Comparable）；其他情况报错
      * [x] limit(n)：取出前n个元素
      * [ ] distinct：使用equals方法进行去重判断
      * [ ] skip(n)：跳过前n个元素
  * [ ] 终结方法：to_list/to_dict/to_tuple/for_each/count
    * [x] to_list/to_tuple：将流元素转换为对应的可迭代类型，返回值为list/tuple
    * [x] to_dict：对二元流元素转换为对应的可迭代类型，返回值为dict
    * [x] for_each：对每个元素都执行该方法，返回值为void
    * [x] count：统计流元素个数，返回值为int
    * [ ] find(func)：返回第一个符合条件的元素，若没有则返回Null
    * [ ] match(func)：如果存在符合条件的元素，返回true，否则返回false
    * [ ] inner_call参数为二元运算的终结方法：
      * [ ] max(func)：若有func且为Callable类型，则使用func进行比较，并返回最大的流元素；否则若流元素均为同一类可比较类型，返回最大的流元素：其他情况均报错
      * [ ] min(func)：若有func且为Callable类型，则使用func进行比较，并返回最小的流元素；否则若流元素均为同一类可比较类型，返回最小的流元素：其他情况均报错
      * [ ] reduce(func, initial)：若initial为空，则第一次计算为func(initial, ele[0])，否则第一次计算为func(ele[0], ele[1])，以此类推计算出所有元素并返回
      * 若仅有一个元素能参与该二元运算，则返回该元素；若没有元素能参与运算，则返回Null
* 类初始化init函数内置
