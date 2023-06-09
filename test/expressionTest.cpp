#include <utility>

#include "../src/lexer.h"
#include "../src/parse/parser.h"
#include "../src/parse/pretty_print.h"
#include "gtest/gtest.h"

ExpressionAST *parseExpression(std::string expr)
{
	int						 size;
	std::string				*tokens = lex(std::move(expr), size);
	std::vector<std::string> tokens_vec(tokens, tokens + size);
	parser					 p(tokens_vec);
	return p.parseExpression();
}

TEST(ExpressionTests, RegularExpressionPrecedenceTests)
{
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
	expressions->insert({"num5() / 2 + 3 == 4 == 5;", "(((num5() / 2) + 3) == (4 == 5))"});
	expressions->insert({"add(5 ,4+3 ,var);", "add(5, (4 + 3), var)"});
	for (auto &expr: *expressions) {
		auto ast = parseExpression(expr.first);
		ASSERT_EQ(exprToString(ast), expr.second) << "Expression: " << exprToString(ast);
	}
}
