#include "lexer.h"
#include "token_iterator.h"
#include <regex>
#include <string>
#include <vector>

/// @param code The code to be lexed
/// @param size a reference to an int that will be set to the size of the array
/// @return A pointer to an array of strings that contain the tokens
/// @note The way im currently doing this is pretty messy as it goes from
///       a s_regex_token_iterator to a string list and then in main gets turned to a vector
///       i will change this at a later date as performance and memory safety right now isn't an issue 03/04/2023
std::string *lex(std::string code, int &size)
{
	using namespace std;
	regex pattern(R"(==|!=|<=|>=|/|::|\+|\-|\*|\|&|=|;|:|,|\{|\}|\(|\)|<|>|\[|\]|[a-zA-Z]+([a-zA-Z0-9])*|\d+)");

	sregex_token_iterator it(code.begin(), code.end(), pattern);
	sregex_token_iterator end;

	size		 = distance(it, end);
	auto *tokens = new std::string[size];

	for (int i = 0; it != end; ++it, ++i) {
		tokens[i] = *it;
	}

	return tokens;
}

/// finite state machine for lexing a string
/// @note incomplete af

lexer::lexer(std::string code)
{
	std::vector<char> v(code.begin(), code.end());
	this->tokens	  = new token_iterator(v);
	this->row		  = 0;
	this->col		  = 0;
	this->keywords	  = std::vector<std::tuple<std::string, int, int>>();
	this->operators	  = {"==", "!=", "<=", ">=", "/", "::", "+", "-", "*", "&", "=",
						 ";",  ":",	 ",",  "{",	 "}", "(",	")", "<", ">", "[", "]"};
	this->firstOpChar = {'=', '!', '<', '>', '/', ':', '+', '-', '*', '&',
						 ';', ',', '{', '}', '(', ')', '<', '>', '[', ']'};
}

std::vector<std::tuple<std::string, int, int>> lexer::lex()
{
	while (!isAtEnd()) {
		char c = peek();
		if (c == ' ' || c == '\t' || c == '\r') {
			next();
		}
		else if (c == '\n') {
			next();
			row++;
			col = 0;
		}
		else if (c == '"') {
			addKeyword(lexString());
		}
		else if (firstOpChar.find(c) != firstOpChar.end()) {
			addKeyword(parseOperator());
		}

		else if (std::isdigit(c)) {
			addKeyword(lexNumber());
		}
		else if (std::isalpha(c)) {
			addKeyword(lexIdentifier());
		}
		else {
			next();
		}
	}
	return keywords;
}

std::string lexer::lexNumber()
{
	std::string num;
	while (std::isdigit(peek())) {
		num += next();
	}
	return num;
}

std::string lexer::lexIdentifier()
{
	std::string id;
	while (std::isalnum(peek())) {
		id += next();
	}
	return id;
}

std::string lexer::lexString()
{
	next(); // consume the opening "
	std::string str;
	while (peek() != '"' && !isAtEnd()) {
		str += next();
	}
	next(); // consume the closing "
	return str;
}

std::string lexer::parseOperator()
{
	std::string op;
	op += next();
	if (operators.find(op + peek()) != operators.end()) {
		op += next();
	}
	return op;
}

/// @return char The next character in the token_iterator and increments the row and col accordingly
/// @note this is kind of like overriding the function but im keeping it in this context as i want to call
///       the next faster

char lexer::next()
{
	auto nx = this->tokens->next();
	this->col++;
	return nx;
}

char lexer::peek() { return this->tokens->peek(); }

bool lexer::isAtEnd() { return this->tokens->isAtEnd(); }

void lexer::addKeyword(const std::string &keyword) { this->keywords.emplace_back(keyword, this->row, this->col); }