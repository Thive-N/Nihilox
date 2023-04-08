#ifndef UNSTABLE_AST_H
#define UNSTABLE_AST_H

#include <memory>
#include <string>
#include <utility>
#include <vector>

class ExpressionAST {
  public:
	virtual ~ExpressionAST() = default;
};

// NumberAST - Node that represents a numerical value
class ExpressionASTNumber: public ExpressionAST {
  public:
	explicit ExpressionASTNumber(std::string val): val(std::move(val)) {}
	std::string val;
};

// VariableAST - Node that represents a variable
class ExpressionASTVariable: public ExpressionAST {
  public:
	explicit ExpressionASTVariable(std::string name): name(std::move(name)) {}
	std::string name;
};

// ExpressionASTBinary - Node that represents a binary operation
class ExpressionASTBinary: public ExpressionAST {
  public:
	ExpressionASTBinary(std::string op, ExpressionAST *lhs, ExpressionAST *rhs): op(std::move(op)), lhs(lhs), rhs(rhs)
	{
	}
	std::string	   op;
	ExpressionAST *lhs;
	ExpressionAST *rhs;
};

// ExpressionASTUnary - Node that represents a unary operation
class ExpressionASTUnary: public ExpressionAST {
  public:
	ExpressionASTUnary(std::string op, ExpressionAST *expr): op(std::move(op)), expr(expr) {}
	std::string	   op;
	ExpressionAST *expr;
};

class ExpressionASTCall: public ExpressionAST {
  public:
	std::string									Call;
	std::vector<ExpressionAST *> Args;
	ExpressionASTCall(std::string Call, std::vector<ExpressionAST *> Args)
		: Call(std::move(Call)), Args(std::move(Args))
	{
	}
};

// PrototypeAST - Node that represents a function prototype such as
// `foo:datatype(bar:datatype, baz:datatype)`
class PrototypeAST {
	std::string				 Name;
	std::vector<std::string> Args;
	PrototypeAST(std::string Name, std::vector<std::string> Args): Name(std::move(Name)), Args(std::move(Args)) {}
};

#endif // UNSTABLE_AST_H
