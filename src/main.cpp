#include "lexer.h"
#include "parse/parser.h"
#include "parse/pretty_print.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char **argv)
{
	if (argc < 2) {
		std::cout << "Please provide a file to run" << std::endl;
		return 1;
	}

	// reading the first argument as a file
	std::ifstream file(argv[1]);
	std::string	  code((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();

	int			 size;
	std::string *tokens = lex("1 / 2 + 3;", size);
	for (int i = 0; i < size; i++) {
		std::cout << tokens[i] << std::endl;
	}
	std::vector<std::string> tokens_vec(tokens, tokens + size);

	parser p(tokens_vec);
	auto   expr = p.parseExpression();
	std::cout << exprToString(expr) << std::endl;
	return 0;
}
