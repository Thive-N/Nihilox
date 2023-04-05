#include <utility>

#include "gtest/gtest.h"
#include "../src/lexer.h"
#include "../src/parse/parser.h"
#include "../src/parse/pretty_print.h"

ExpressionAST* parseExpression(std::string expr){
	int size;
	std::string *tokens = lex(std::move(expr), size);
	std::vector<std::string> tokens_vec(tokens, tokens+size);
	parser p(tokens_vec);
	return p.parseExpression();
}

TEST(GeneratedTest, expressionTest) {
	auto expressions = new std::unordered_map<std::string, std::string>();
	expressions->insert({"1 + 2 * 3;", "(1 + (2 * 3))"});
	expressions->insert({"1 * 2 + 3;", "((1 * 2) + 3)"});
	expressions->insert({"1 / 2 + 3;", "((1 / 2) + 3)"});
	expressions->insert({"1 == 2 + 3;", "(1 == (2 + 3))"});
	expressions->insert({"1 + 2 == 3;", "((1 + 2) == 3)"});
	expressions->insert({"1 + 2 * 3 == 4;", "((1 + (2 * 3)) == 4)"});
	expressions->insert({"1 * 2 + 3 == 4;", "(((1 * 2) + 3) == 4)"});
	expressions->insert({"1 / 2 + 3 == 4;", "(((1 / 2) + 3) == 4)"});
	expressions->insert({"1 == 2 + 3 == 4;", "(1 == ((2 + 3) == 4))"});
	expressions->insert({"1 + 2 == 3 == 4;", "((1 + 2) == (3 == 4))"});
	expressions->insert({"1 + 2 * 3 == 4 == 5;", "((1 + (2 * 3)) == (4 == 5))"});
	expressions->insert({"1 * 2 + 3 == 4 == 5;", "(((1 * 2) + 3) == (4 == 5))"});

	for (auto &expr : *expressions) {
		auto ast = parseExpression(expr.first);
		ASSERT_EQ(exprToString(ast),expr.second) << "Expression: " << exprToString(ast);
	}
}
