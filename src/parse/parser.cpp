#include "parser.h"

#include "../ast.h"
#include "../token_iterator.h"
#include <regex>
#include <utility>

/// @param tokens_ A Reference to a vector of strings that contain the tokens
/// @note This moves the tokens_ to the parser class
parser::parser(std::vector<std::string> &tokens_)
{
	tokens = new token_iterator(tokens_);
}


StatementASTVariableDeclaration *parser::parseVariableDeclaration()
{
	tokens->next(); // consume the let keyword
	auto		name = tokens->next();
	std::string type = "auto";
	if (tokens->peek() == "::") {
		tokens->next(); // consume the ::
		type = tokens->next();
	}
	if (tokens->next() != "=")
		throw std::runtime_error("Error while parsing variable declaration expected '=' but got '" + tokens->getToken() + "'");
	auto expr = parseExpression();
	if (tokens->next() != ";")
		throw std::runtime_error("Error while parsing variable declaration expected ';' but got '" + tokens->getToken() + "'");
	return new StatementASTVariableDeclaration(name, type, *expr);
}

StatementASTConditional *parser::parseConditional()
{
	tokens->next(); // consume the if keyword
	auto condition = parseExpression();
	auto trueBlock = parseBlock();
	if (tokens->peek() != "else") {
		return new StatementASTConditional(*condition, *trueBlock);
	} else {
		tokens->next(); // consume the else keyword
		auto falseBlock = parseBlock();
		return new StatementASTConditional(*condition, *trueBlock, *falseBlock);
	}
}
StatementASTLoop *parser::parseLoop()
{
	tokens->next(); // consume the while keyword
	auto condition = parseExpression();
	auto block	 = parseBlock();
	return new StatementASTLoop(*condition, *block);
}

PrototypeAST *parser::parsePrototype()
{
	auto name = tokens->next();
	if (tokens->next() != "(")
		throw std::runtime_error("Error while parsing prototype expected '(' but got '" + tokens->getToken() + "'");
	std::vector<std::string> args;
	while (tokens->peek() != ")") {
		args.push_back(tokens->next());
		if (tokens->peek() == ",") {
			tokens->next();
		}
	}
	tokens->next(); // consume the )
	return new PrototypeAST(name, args);
}

FunctionAST *parser::parseReturn()
{
	tokens->next();
	auto proto = parsePrototype();
	auto body = parseBlock();
	return new FunctionAST(*proto, *body);
}
StatementAST *parser::parseStatement()
{
	if (tokens->peek() == "let") {
		return parseVariableDeclaration();
	}
	else {
		throw std::runtime_error("Error while parsing statement expected 'let' but got '" + tokens->peek() + "'");
	}
}
BlockAST *parser::parseBlock()
{
	auto *statements = new std::vector<StatementAST *>;
	while (tokens->peek() != "}") {
		statements->push_back(parseStatement());
	}
	tokens->next(); // consume the }
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
	auto token = tokens->next();

	if (isOperator(token) && getPrecedence(token) == precedence) {
		auto rhs = parseExpression(precedence);
		return new ExpressionASTBinary(token, lhs, rhs);
	}
	else {
		tokens->back(); // move the iterator back by one
		return lhs;
	}
}

/// @return ExpressionAST* A pointer to an ExpressionAST object that contains the parsed primary expression
/// @note This will parse a primary expression which is either a number, a variable, or a parenthesized expression
///       or a unary expression or a function call with expressions as arguments
ExpressionAST *parser::parsePrimary()
{
	auto nx = tokens->next();
	if (nx == "(") {
		auto expr = parseExpression();
		if (tokens->next() != ")")
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
		if (tokens->peek() == "(") {
			std::vector<ExpressionAST *> args;
			tokens->next(); // consume the '('
			while (tokens->peek() != ")") {
				args.push_back(parseExpression());
				if (tokens->peek() == ",") {
					tokens->next(); // consume the ','
				}
			}
			tokens->next(); // consume the ')'
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