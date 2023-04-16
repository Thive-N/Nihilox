#ifndef UNSTABLE_PARSER_H
#define UNSTABLE_PARSER_H

#include "../ast.h"
#include "../token_iterator.h"

class parser {
  public:
	explicit parser(std::vector<std::string> &);

	virtual ~parser() = default;
	token_iterator 					*tokens;

	ExpressionAST					*parseExpression();
	ExpressionAST					*parseExpression(int precedence);
	ExpressionAST					*parsePrimary();
	int								 getPrecedence(const std::string &op);
	bool							 isOperator(const std::string &op);
	StatementAST					*parseStatement();
	BlockAST						*parseBlock();
	StatementASTVariableDeclaration *parseVariableDeclaration();
	StatementASTConditional			*parseConditional();
	StatementASTLoop				*parseLoop();
	FunctionAST						*parseReturn();
	PrototypeAST					*parsePrototype();
};

#endif // UNSTABLE_PARSER_H
