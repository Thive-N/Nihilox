#![allow(dead_code)]
/// representation of a expression in AST
#[derive(Debug, PartialEq)]
pub enum ExpressionASTNode {
    Number {
        value: i32,
    },
    Variable {
        name: String,
    },
    Binary {
        left: Box<ExpressionASTNode>,
        op: BinaryOperator,
        right: Box<ExpressionASTNode>,
    },
    Unary {
        op: UnaryOperator,
        operand: Box<ExpressionASTNode>,
    },
    Paren {
        expr: Box<ExpressionASTNode>,
    },
}

#[derive(Debug, PartialEq)]
pub enum BinaryOperator {
    Add,
    Subtract,
    Multiply,
    Divide,
}

#[derive(Debug, PartialEq)]
pub enum UnaryOperator {
    Negate,
}
