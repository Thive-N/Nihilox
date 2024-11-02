use std::iter::Peekable;

use crate::ast::*;
use crate::lexer::Token;

pub fn parse_term<'a, I>(tokenstream: &mut Peekable<I>) -> ExpressionASTNode
where
    I: Iterator<Item = &'a Token>,
{
    // expected to have at least one token
    if tokenstream.peek().is_none() {
        panic!("Unexpected end of input");
    }
    let token = tokenstream.next().unwrap();

    // if the token is a number, return a Number node
    if token.token.chars().next().unwrap().is_numeric() {
        return ExpressionASTNode::Number {
            value: token.token.parse().unwrap(),
        };
    // if the token is a parenthesis, parse the expression inside the parenthesis
    } else if token.token == "(" {
        let expr = parse_expression(tokenstream);
        if tokenstream.next().unwrap().token != ")" {
            panic!("Expected closing parenthesis");
        }
        return ExpressionASTNode::Paren {
            expr: Box::new(expr),
        };
    } else if is_operator(token) {
        match token.token.as_str() {
            "+" => {
                return ExpressionASTNode::Unary {
                    op: UnaryOperator::Negate,
                    operand: Box::new(parse_term(tokenstream)),
                };
            }
            "-" => {
                return ExpressionASTNode::Unary {
                    op: UnaryOperator::Negate,
                    operand: Box::new(parse_term(tokenstream)),
                };
            }
            _ => panic!("Unexpected operator"),
        }
    } else {
        return ExpressionASTNode::Variable {
            name: token.token.to_string(),
        };
    }
}

pub fn parse_expression<'a, I>(tokenstream: &mut Peekable<I>) -> ExpressionASTNode
where
    I: Iterator<Item = &'a Token>,
{
    let mut left = parse_term(tokenstream);
    while let Some(token) = tokenstream.peek() {
        match token.token.as_str() {
            "+" => {
                tokenstream.next();
                left = ExpressionASTNode::Binary {
                    left: Box::new(left),
                    op: BinaryOperator::Add,
                    right: Box::new(parse_term(tokenstream)),
                };
            }
            "-" => {
                tokenstream.next();
                left = ExpressionASTNode::Binary {
                    left: Box::new(left),
                    op: BinaryOperator::Subtract,
                    right: Box::new(parse_term(tokenstream)),
                };
            }
            "*" => {
                tokenstream.next();
                left = ExpressionASTNode::Binary {
                    left: Box::new(left),
                    op: BinaryOperator::Multiply,
                    right: Box::new(parse_term(tokenstream)),
                };
            }
            "/" => {
                tokenstream.next();
                left = ExpressionASTNode::Binary {
                    left: Box::new(left),
                    op: BinaryOperator::Divide,
                    right: Box::new(parse_term(tokenstream)),
                };
            }
            _ => break,
        }
    }
    left
}

pub fn is_operator(token: &Token) -> bool {
    match token.token.as_str() {
        "+" | "-" | "*" | "/" => true,
        _ => false,
    }
}

#[cfg(test)]
mod tests {
    use crate::lexer;

    use super::*;

    #[test]
    fn test_parse_expression() {
        let tokens = lexer::lex("1 + 2");
        let mut tokenstream = tokens.iter().peekable();
        let ast = parse_expression(&mut tokenstream);
        assert_eq!(
            ast,
            ExpressionASTNode::Binary {
                left: Box::new(ExpressionASTNode::Number { value: 1 }),
                op: BinaryOperator::Add,
                right: Box::new(ExpressionASTNode::Number { value: 2 }),
            }
        );
    }

    #[test]
    fn test_parse_expression_complex() {
        let tokens = lexer::lex("1 + (-2 - -4)");
        let mut tokenstream = tokens.iter().peekable();
        let ast = parse_expression(&mut tokenstream);

        assert_eq!(
            ast,
            ExpressionASTNode::Binary {
                left: Box::new(ExpressionASTNode::Number { value: 1 }),
                op: BinaryOperator::Add,
                right: Box::new(ExpressionASTNode::Paren {
                    expr: Box::new(ExpressionASTNode::Binary {
                        left: Box::new(ExpressionASTNode::Unary {
                            op: UnaryOperator::Negate,
                            operand: Box::new(ExpressionASTNode::Number { value: 2 })
                        }),
                        op: BinaryOperator::Subtract,
                        right: Box::new(ExpressionASTNode::Unary {
                            op: UnaryOperator::Negate,
                            operand: Box::new(ExpressionASTNode::Number { value: 4 }),
                        }),
                    }),
                }),
            }
        );
    }
}
