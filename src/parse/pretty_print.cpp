#include "ast.h"

std::string exprToString(ExpressionAST *expr)
{
	std::string str;
	if (typeid(*expr) == typeid(ExpressionASTBinary)) {
		auto binExpr = dynamic_cast<ExpressionASTBinary *>(expr);
		str += "(" + exprToString(binExpr->lhs) + " " + binExpr->op + " " + exprToString(binExpr->rhs) + ")";
	}

	else if (typeid(*expr) == typeid(ExpressionASTNumber)) {
		auto numExpr = dynamic_cast<ExpressionASTNumber *>(expr);
		str += std::to_string(std::stoi(numExpr->val));
	}

	else if (typeid(*expr) == typeid(ExpressionASTUnary)) {
		auto unExpr = dynamic_cast<ExpressionASTUnary *>(expr);
		str += "(" + unExpr->op + " " + exprToString(unExpr->expr) + ")";
	}

	else if (typeid(*expr) == typeid(ExpressionASTVariable)) {
		auto varExpr = dynamic_cast<ExpressionASTVariable *>(expr);
		str += varExpr->name;
	}

	else if (typeid(*expr) == typeid(ExpressionASTCall)) {
		auto callExpr = dynamic_cast<ExpressionASTCall *>(expr);
		str += callExpr->Call + "(";	  // function name + (
		auto len = callExpr->Args.size(); // get number of arguments
		for (auto &arg: callExpr->Args) { // iterate over arguments
			str += exprToString(arg);	  // convert argument to string and append to str
			if (len-- != 1) str += ", ";  // if not last argument, append a comma
		}
		str += ")"; // append closing parenthesis
	}
	return str;
}