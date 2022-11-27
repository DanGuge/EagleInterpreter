## AST

### Stmt

```
Class 		: Token name, Variable super_class, List<Var> members, List<Function> methods
Function 	: Token name, List<Token> params, List<Stmt> body
Var 		: Token name, Expr initializer
If 			: Expr condition, Stmt then_branch, Stmt else_branch
While 		: Expr condition, Stmt body
For         : Stmt initializer, Expr condition, Stmt increment, Stmt body 
Expression 	: Expr expression
Print 		: Expr print_value
Return 		: Expr return_value, int line
Break 		: Token keyword
Continue 	: Token keyword
Block 		: List<Stmt> statements
Empty       :
```

### Expr

```
Assign 			: Token name, Token operator, Expr value
Ternary 		: Expr condition, Expr then_expr, Expr else_expr
Compare 		: Expr first, List<Pair<Token, Expr>> others
Binary 			: Expr left, Token operator, Expr right
Unary 			: Token operator, Expr expr
Call 			: Expr callee, List<Expr> arguments, int line
Literal 		: Object value
Variable 		: Token name
Stream 			: Expr expr, List<Pair<Token, Expr>> operations
Switch 			: Expr expr, List<Pair<Expr, Expr>> case_results, Expr default_result
Lambda			: List<Token> params, Stmt body
InstanceSet 	: Expr object, Token name, Token operator, Expr value
InstanceGet 	: Expr object, Token name
ContainerSet 	: Expr container, Expr subscript, Token operator, Expr value
ContainerGet 	: Expr container, Expr subscript
This 			: Token keyword
Super 			: Token keyword, Token method
Sequence 		: Token type, List<Expr> elements
Associative 	: Token type, List<Pair<Expr, Expr>> elements
```

