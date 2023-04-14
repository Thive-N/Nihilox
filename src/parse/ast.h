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
	~ExpressionASTNumber() override = default;
	explicit ExpressionASTNumber(std::string val): val(std::move(val)) {}
	std::string val;
};

// VariableAST - Node that represents a variable
class ExpressionASTVariable: public ExpressionAST {
  public:
	~ExpressionASTVariable() override = default;
	explicit ExpressionASTVariable(std::string name): name(std::move(name)) {}
	std::string name;
};

// ExpressionASTBinary - Node that represents a binary operation
class ExpressionASTBinary: public ExpressionAST {
  public:
	~ExpressionASTBinary() override = default;
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
	~ExpressionASTUnary() override = default;
	ExpressionASTUnary(std::string op, ExpressionAST *expr): op(std::move(op)), expr(expr) {}
	std::string	   op;
	ExpressionAST *expr;
};

class ExpressionASTCall: public ExpressionAST {
  public:
	~ExpressionASTCall() override = default;
	ExpressionASTCall(std::string Call, std::vector<ExpressionAST *> Args): Call(std::move(Call)), Args(std::move(Args))
	{
	}
	std::string					 Call;
	std::vector<ExpressionAST *> Args;
};

// end of ExpressionAST nodes

// PrototypeAST - Node that represents a function prototype such as
// `foo:datatype(bar:datatype, baz:datatype)`
class PrototypeAST {
	std::string				 Name;
	std::vector<std::string> Args;
	PrototypeAST(std::string Name, std::vector<std::string> Args): Name(std::move(Name)), Args(std::move(Args)) {}
};

class StatementAST {
  public:
	virtual ~StatementAST() = default;
};

class StatementASTVariableDeclaration: public StatementAST {
  public:
	~StatementASTVariableDeclaration() override = default;
	StatementASTVariableDeclaration(std::string varName, std::string varType, const ExpressionAST &expression)
		: varName(std::move(varName)), varType(std::move(varType)), expr(expression)
	{
	}

	std::string	  varName;
	std::string	  varType;
	ExpressionAST expr;
};

class StatementASTConditional: public StatementAST {
  public:
	~StatementASTConditional() override = default;
	StatementASTConditional(const ExpressionAST &condition, const ExpressionAST &trueBranch,
							const ExpressionAST &falseBranch)
		: condition(condition), trueBranch(trueBranch), falseBranch(falseBranch)
	{
	}
	ExpressionAST condition;
	ExpressionAST trueBranch;  // temporary
	ExpressionAST falseBranch; // temporary
};

class BlockAST {
  public:
	~BlockAST() = default;
	explicit BlockAST(std::vector<StatementAST *> statements): statements(std::move(statements)) {}
	std::vector<StatementAST *> statements;
};

#endif // UNSTABLE_AST_H
