#include "../src/lexer.h"
#include "gtest/gtest.h"

/// Run the lexer on the given expressions and compare the output to the expected output.
/// to reduce repeated code
/// @param expressions A map of expressions to their expected output.
void runGivenTests(const std::unordered_map<std::string, std::vector<std::string>> &expressions)
{
	for (auto expr: expressions) {
		auto *l		 = new lexer(expr.first);
		auto  tokens = l->lex();

		ASSERT_EQ(tokens.size(), expr.second.size());
		for (int i = 0; i < tokens.size(); i++) {
			EXPECT_EQ(std::get<0>(tokens[i]), expr.second[i]);
		}
	}
}

void runGivenTests(const std::unordered_map<std::string, std::vector<std::tuple<std::string,int,int>>> &expressions){
	for (const auto& expr: expressions) {
		auto *l		 = new lexer(expr.first);
		auto  tokens = l->lex();
		ASSERT_EQ(tokens.size(), expr.second.size());
		for (int i = 0; i < tokens.size(); i++) {
			EXPECT_EQ(std::get<0>(tokens[i]), std::get<0>(expr.second[i]));
			EXPECT_EQ(std::get<1>(tokens[i]), std::get<1>(expr.second[i]));
			EXPECT_EQ(std::get<2>(tokens[i]), std::get<2>(expr.second[i]));
		}
	}
}
/// Basic Expression Tests
TEST(LexerTests, RegularExpressionTests)
{
	auto expressions = new std::unordered_map<std::string, std::vector<std::string>>();
	expressions->insert({"1 + 2 * 3;", {"1", "+", "2", "*", "3", ";"}});
	expressions->insert({"1 * 2 + 3;", {"1", "*", "2", "+", "3", ";"}});
	expressions->insert({"1 / 2 + 3;", {"1", "/", "2", "+", "3", ";"}});
	expressions->insert({"1 == 2 + 3;", {"1", "==", "2", "+", "3", ";"}});
	expressions->insert({"1 + 2 == 3;", {"1", "+", "2", "==", "3", ";"}});
	expressions->insert({"1 + 2 * 3 == 4;", {"1", "+", "2", "*", "3", "==", "4", ";"}});
	expressions->insert({"1 * 2 + 3 == 4;", {"1", "*", "2", "+", "3", "==", "4", ";"}});

	runGivenTests(*expressions);
}

/// Basic Expression Tests With Unnatural Spacing
TEST(LexerTests, UnnaturalExpressionSpacing)
{
	auto expressions = new std::unordered_map<std::string, std::vector<std::string>>();
	expressions->insert({"  1   + 2   *         3;", {"1", "+", "2", "*", "3", ";"}});
	expressions->insert({"1*  2+  3    ;", {"1", "*", "2", "+", "3", ";"}});
	expressions->insert({"1 / 2+  3 ;", {"1", "/", "2", "+", "3", ";"}});
	expressions->insert({"1  ==   2 + 3;", {"1", "==", "2", "+", "3", ";"}});
	expressions->insert({"1 +2 == 3;", {"1", "+", "2", "==", "3", ";"}});
	expressions->insert({"1+  2* 3==4;", {"1", "+", "2", "*", "3", "==", "4", ";"}});
	expressions->insert({"1   * 2+    3==4;", {"1", "*", "2", "+", "3", "==", "4", ";"}});

	runGivenTests(*expressions);
}

/// Tests to ensure that Strings and Numbers are split correctly.
TEST(LexerTests, StringDigitSplitTest)
{
	auto expressions = new std::unordered_map<std::string, std::vector<std::string>>();
	expressions->insert({"1+2;", {"1", "+", "2", ";"}});
	expressions->insert({"1+2+string;", {"1", "+", "2", "+", "string", ";"}});
	expressions->insert({"1+2+string(5)+3;", {"1", "+", "2", "+", "string", "(", "5", ")", "+", "3", ";"}});
	expressions->insert({"fn function232dc(datatype,datatype){int rewrew342}",
						 {"fn", "function232dc", "(", "datatype", ",", "datatype", ")", "{", "int", "rewrew342", "}"}});
	expressions->insert({"3423fe", {"3423", "fe"}});

	runGivenTests(*expressions);
}

/// This test is to ensure that the lexer can handle the case where a token is a substring of another token.
/// Some of these tests are ultra unrealistic and the others are basic,
/// but they are here to ensure that the lexer can handle them and the lexer is consistent as ill be changing the regex
/// out for a finite state machine.
TEST(LexerTests, CorrectTokenPrecedence)
{
	auto expressions = new std::unordered_map<std::string, std::vector<std::string>>();
	expressions->insert({"!=", {"!="}});
	expressions->insert({"==", {"=="}});
	expressions->insert({"<=", {"<="}});
	expressions->insert({">=", {">="}});
	expressions->insert({"::", {"::"}});

	expressions->insert({">==", {">=", "="}});
	expressions->insert({"<==", {"<=", "="}});
	expressions->insert({">===", {">=", "=="}});
	expressions->insert({"<===", {"<=", "=="}});

	expressions->insert({"===", {"==", "="}});
	expressions->insert({"====", {"==", "=="}});

	expressions->insert({"!==", {"!=", "="}});
	expressions->insert({"!===", {"!=", "=="}});

	runGivenTests(*expressions);
}

TEST(LexerTests, LineRowNumberTest){
	auto expressions = new std::unordered_map<std::string, std::vector<std::tuple<std::string,int,int>>>();
	expressions->insert({"1+2;", {{"1",1,1},{"+",1,2},{"2",1,3},{";",1,4}}});
	expressions->insert({"1+2+string;", {{"1",1,1},{"+",1,2},{"2",1,3},{"+",1,4},{"string",1,10},{";",1,11}}});
	runGivenTests(*expressions);
}