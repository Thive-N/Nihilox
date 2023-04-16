#include "parser.h"

#include "../ast.h"
#include <regex>
#include <utility>

/// @param tokens_ A Reference to a vector of strings that contain the tokens
/// @note This moves the tokens_ to the parser class
parser::parser(std::vector<std::string> &tokens_)
{
	tokens = std::move(tokens_); // move the tokens_ to the parser class
	begin  = tokens.begin();	 // set the begin iterator to the beginning of the vector
	end	   = tokens.end();		 // set the end iterator to the end of the vector
}

/// @return The next token in the vector
/// @note The iterator is incremented by one but moved back to the original position
std::string parser::peek()
{
	if (notAtStart) {
		begin++;
		auto temp = *begin;
		begin--;
		return temp;
	}
	return *begin;
}

/// @return The next token in the vector
/// @note The iterator is incremented by one
std::string parser::next()
{
	if (notAtStart) {
		begin++;	   // increment the iterator
		return *begin; // return the token
	}
	notAtStart = true;
	return *begin;
}

/// @note moves the iterator back by one
/// useful in expression parsing so that no more than the required tokens are consumed
void parser::back() { begin--; }

/// @return if the iterator is at the end of the vector
/// @note This will check if the iterator is at the end of the vector or beyond
bool parser::isAtEnd() { return begin > end; }

/// @return The current token
/// @note This will return the current token regardless of where the iterator is
std::string parser::getToken() { return *begin; }

StatementASTVariableDeclaration *parser::parseVariableDeclaration()
{
	next(); // consume the let keyword
	auto		name = next();
	std::string type = "auto";
	if (peek() == "::") {
		next(); // consume the ::
		type = next();
	}
	if (next() != "=")
		throw std::runtime_error("Error while parsing variable declaration expected '=' but got '" + getToken() + "'");
	auto expr = parseExpression();
	if (next() != ";")
		throw std::runtime_error("Error while parsing variable declaration expected ';' but got '" + getToken() + "'");
	return new StatementASTVariableDeclaration(name, type, *expr);
}

StatementASTConditional *parser::parseConditional()
{
	next(); // consume the if keyword
	auto condition = parseExpression();
	auto trueBlock = parseBlock();
	if (peek() != "else") {
		return new StatementASTConditional(*condition, *trueBlock);
	} else {
		next(); // consume the else keyword
		auto falseBlock = parseBlock();
		return new StatementASTConditional(*condition, *trueBlock, *falseBlock);
	}
}
StatementASTLoop *parser::parseLoop()
{
	next(); // consume the while keyword
	auto condition = parseExpression();
	auto block	 = parseBlock();
	return new StatementASTLoop(*condition, *block);
}

PrototypeAST *parser::parsePrototype()
{
	auto name = next();
	if (next() != "(")
		throw std::runtime_error("Error while parsing prototype expected '(' but got '" + getToken() + "'");
	std::vector<std::string> args;
	while (peek() != ")") {
		args.push_back(next());
		if (peek() == ",") {
			next();
		}
	}
	next(); // consume the )
	return new PrototypeAST(name, args);
}

FunctionAST *parser::parseReturn()
{
	next();
	auto proto = parsePrototype();
	auto body = parseBlock();
	return new FunctionAST(*proto, *body);
}
StatementAST *parser::parseStatement()
{
	if (peek() == "let") {
		return parseVariableDeclaration();
	}
	else {
		throw std::runtime_error("Error while parsing statement expected 'let' but got '" + peek() + "'");
	}
}
BlockAST *parser::parseBlock()
{
	auto *statements = new std::vector<StatementAST *>;
	while (peek() != "}") {
		statements->push_back(parseStatement());
	}
	next(); // consume the }
	auto *block = new BlockAST(*statements);
	return block;
}

/// @return ExpressionAST* A pointer to an ExpressionAST object that contains the parsed expression tree
ExpressionAST *parser::parseExpression() { return parseExpression(0); }
ExpressionAST *parser::parseExpression(int precedence)
{
	if (precedence >= 3) {
		return parsePrimary();
	}

	auto lhs   = parseExpression(precedence + 1);
	auto token = next();

	if (isOperator(token) && getPrecedence(token) == precedence) {
		auto rhs = parseExpression(precedence);
		return new ExpressionASTBinary(token, lhs, rhs);
	}
	else {
		back(); // move the iterator back by one
		return lhs;
	}
}

/// @return ExpressionAST* A pointer to an ExpressionAST object that contains the parsed primary expression
/// @note This will parse a primary expression which is either a number, a variable, or a parenthesized expression
///       or a unary expression or a function call with expressions as arguments
ExpressionAST *parser::parsePrimary()
{
	auto nx = next();
	if (nx == "(") {
		auto expr = parseExpression();
		if (next() != ")")
			throw std::runtime_error("Error while parsing primary expression expected ')' but got '" + nx + "'");
		return expr;
	}

	else if (nx == "-") {
		return new ExpressionASTUnary("-", parsePrimary());
	}

	else if (std::regex_match(nx, std::regex("\\d+"))) {
		return new ExpressionASTNumber(nx);
	}

	else if (std::regex_match(nx, std::regex("[a-zA-Z]([a-zA-Z0-9])*"))) {
		if (peek() == "(") {
			std::vector<ExpressionAST *> args;
			next(); // consume the '('
			while (peek() != ")") {
				args.push_back(parseExpression());
				if (peek() == ",") {
					next(); // consume the ','
				}
			}
			next(); // consume the ')'
			return new ExpressionASTCall(nx, args);
		}
		else {
			return new ExpressionASTVariable(nx);
		}
	}

	else
		throw std::runtime_error(
			"Error while parsing primary expression expected '(' or '-' or number or variable but got " + nx);
}

int parser::getPrecedence(const std::string &op)
{
	if (op == "==" || op == "!=" || op == "<" || op == ">" || op == "<=" || op == ">") {
		return 0;
	}
	if (op == "+" || op == "-") {
		return 1;
	}
	if (op == "*" || op == "/") {
		return 2;
	}
	else {
		throw std::runtime_error("Error while getting precedence of operator '" + op + "' is not an operator");
	}
}

bool parser::isOperator(const std::string &op)
{
	return op == "+" || op == "-" || op == "*" || op == "/" || op == "==" || op == "!=" || op == "<" || op == ">" ||
		   op == "<=" || op == ">=";
}