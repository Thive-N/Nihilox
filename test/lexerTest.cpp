#include <utility>

#include "gtest/gtest.h"
#include "../src/lexer.h"

TEST (BasicTests, LexerTest) {
	auto expressions = new std::unordered_map<std::string, std::string*>();
	expressions->insert({"1 + 2 * 3;", new std::string[]{"1", "+", "2", "*", "3", ";"}});
	expressions->insert({"1 * 2 + 3;", new std::string[]{"1", "*", "2", "+", "3", ";"}});
	expressions->insert({"1 / 2 + 3;", new std::string[]{"1", "/", "2", "+", "3", ";"}});
	expressions->insert({"1 == 2 + 3;", new std::string[]{"1", "==", "2", "+", "3", ";"}});
	expressions->insert({"1 + 2 == 3;", new std::string[]{"1", "+", "2", "==", "3", ";"}});
	expressions->insert({"1 + 2 * 3 == 4;", new std::string[]{"1", "+", "2", "*", "3", "==", "4", ";"}});
	expressions->insert({"1 * 2 + 3 == 4;", new std::string[]{"1", "*", "2", "+", "3", "==", "4", ";"}});

	for (auto &expr : *expressions) {
		int size;
		std::string *tokens = lex(expr.first, size);
		for (int i = 0; i < expr.second->size(); i++) {
			if (i>=size)
				FAIL() << "Expected " << expr.second->size() << " tokens, got " << size;
			ASSERT_EQ(tokens[i], expr.second[i]);
		}
	}

}
