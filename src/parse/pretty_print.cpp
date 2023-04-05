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
	return str;
}