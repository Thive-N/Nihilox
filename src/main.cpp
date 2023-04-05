#include "lexer.h"
#include "parse/parser.h"
#include "parse/pretty_print.h"
#include <iostream>
#include <string>
#include <vector>

int main()
{
	int						 size;
	std::string				*tokens = lex("1 / 2 + 3;", size);
	for (int i = 0; i < size; i++) {
		std::cout << tokens[i] << std::endl;
	}
	std::vector<std::string> tokens_vec(tokens, tokens + size);

	parser p(tokens_vec);
	auto   expr = p.parseExpression();
	std::cout << exprToString(expr) << std::endl;
	return 0;
}
