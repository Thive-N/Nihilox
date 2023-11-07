#![allow(dead_code)]
/// representation of a expression in AST

pub enum ExpressionASTNode {
    Number,
    Variable,
    Binary,
    Unary,
}

/// representation of a number in an expression tree
pub struct Number {
    pub value: i64,
}

/// representation of a variable in an expression tree
pub struct Variable {
    pub name: String,
}

/// representation of a binary operation in an expression tree
pub struct Binary {
    pub op: String,
    pub lhs: ExpressionASTNode,
    pub rhs: ExpressionASTNode,
}

/// representation of a unary operation in an expression tree
pub struct Unary {
    pub op: String,
    pub expression: ExpressionASTNode,
}

// representation of a function call in an expression tree
pub struct FunctionCall {
    pub name: String,
    pub args: Vec<ExpressionASTNode>,
}
