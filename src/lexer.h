#ifndef UNSTABLE_LEXER_H
#define UNSTABLE_LEXER_H

#include "token_iterator.h"
#include <string>
#include <tuple>
#include <vector>
#include <set>

std::string *lex(std::string code, int &size);

class lexer {
  public:
	explicit lexer(std::string &code);
	~lexer() = default;

	std::vector<std::tuple<std::string, int, int>> lex();

  private:
	std::set<std::string>					       operators;
	std::set<char>							       firstOpChar;
	void										   addKeyword(const std::string &keyword);
	char										   next();
	char										   peek();
	bool										   isAtEnd();
	int											   row;
	int											   col;
	token_iterator<char>						  *tokens;
	std::vector<std::tuple<std::string, int, int>> keywords;
	std::string									   lexString();
	std::string									   parseOperator();
	std::string									   lexNumber();
	std::string									   lexIdentifier();
};
#endif // UNSTABLE_LEXER_H