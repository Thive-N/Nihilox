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

class StatementAST {
  public:
	virtual ~StatementAST() = default;
};

class BlockAST {
  public:
	~BlockAST() = default;
	explicit BlockAST(std::vector<StatementAST *> statements): statements(std::move(statements)) {}
	std::vector<StatementAST *> statements;
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
	StatementASTConditional(const ExpressionAST &condition, const BlockAST &trueBranch,
							const BlockAST &falseBranch)
		: condition(condition), trueBranch(trueBranch), falseBranch(falseBranch)
	{
	}
	ExpressionAST condition;
	BlockAST trueBranch;
	BlockAST falseBranch;
};

class StatementASTLoop: public StatementAST {
  public:
	~StatementASTLoop() override = default;
	StatementASTLoop(const ExpressionAST &condition, const BlockAST &body): condition(condition), body(body) {}
	ExpressionAST condition;
	BlockAST body;
};

/// PrototypeAST - Node that represents a function prototype such as
/// `foo:datatype(bar:datatype, baz:datatype)`
/// @todo: add support for function return types and arguments types
class PrototypeAST {
  public:
	PrototypeAST(std::string Name, std::vector<std::string> Args): Name(std::move(Name)), Args(std::move(Args)) {}
	std::string				 Name;
	std::vector<std::string> Args;
};

// FunctionAST - Node that represents a function definition
class FunctionAST {
  public:
	~FunctionAST() = default;
	FunctionAST(std::unique_ptr<PrototypeAST> proto, std::unique_ptr<BlockAST> body)
		: proto(std::move(proto)), body(std::move(body))
	{
	}
	std::unique_ptr<PrototypeAST> proto;
	std::unique_ptr<BlockAST>	 body;
};

#endif // UNSTABLE_AST_H
