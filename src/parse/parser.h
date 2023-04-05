#ifndef UNSTABLE_PARSER_H
#define UNSTABLE_PARSER_H

#include "ast.h"

class parser {
  public:
	explicit parser(std::vector<std::string> &);

	virtual ~parser() = default;

	//  private:
	//    helper functions
	std::string peek();

	std::string next();

	void back();

	std::string getToken();

	bool notAtStart = false;
	bool isAtEnd();

	std::vector<std::string>::const_iterator begin;
	std::vector<std::string>::const_iterator end;
	std::vector<std::string>				 tokens;

	ExpressionAST *parseExpression();
	ExpressionAST *parseExpression(int precedence);
	ExpressionAST *parsePrimary();
	int			   getPrecedence(const std::string &op);
	bool		   isOperator(const std::string &op);
};

#endif // UNSTABLE_PARSER_H
