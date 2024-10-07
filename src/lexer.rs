#![allow(dead_code)]

use std::fmt::Display;
#[derive(Debug)]
pub struct Token {
    token: String,
    row: u32,
    col: u32,
}

impl Display for Token {
    fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
        write!(
            f,
            "Token: {} at row: {}, col: {}",
            self.token, self.row, self.col
        )
    }
}
fn push_token(tokens: &mut Vec<Token>, token: &mut String, row: u32, col: u32) -> bool {
    if !token.is_empty() {
        tokens.push(Token {
            token: token.clone(),
            row,
            col,
        });
        token.clear();
        return true;
    }
    false
}

pub fn lex(raw_code: &str) -> Vec<Token> {
    let mut tokens = Vec::new();
    let mut row = 1;
    let mut col = 0;
    let mut token = String::new();
    let mut char_iter = raw_code.chars().peekable();

    while let Some(c) = char_iter.next() {
        match c {
            ' ' | '\t' | '\r' => {
                push_token(&mut tokens, &mut token, row, col);
                col += 1;
            }

            '\n' => {
                push_token(&mut tokens, &mut token, row, col);
                row += 1;
                col = 0;
            }

            // boolean operators
            '<' | '>' | '=' | '!' => {
                push_token(&mut tokens, &mut token, row, col);
                col += 1;
                token.push(c);
                if char_iter.peek() == Some(&'=') {
                    char_iter.next();
                    token.push('=');
                    push_token(&mut tokens, &mut token, row, col);
                    col += 1;
                } else {
                    push_token(&mut tokens, &mut token, row, col);
                }
            }

            // single character tokens
            '+' | '-' | '*' | '/' | '%' | ':' | ';' => {
                push_token(&mut tokens, &mut token, row, col);
                col += 1;
                token.push(c);
                push_token(&mut tokens, &mut token, row, col);
                col += 1;
            }

            // handle char strings
            '\'' => {
                let mut escape = false;
                push_token(&mut tokens, &mut token, row, col);
                token.push(c);
                // if the next character is a backslash, we need to escape it
                if char_iter.peek() == Some(&'\\') {
                    escape = true;
                    token.push(char_iter.next().unwrap());
                    token.push(char_iter.next().unwrap());
                } else {
                    token.push(char_iter.next().unwrap());
                }
                token.push(char_iter.next().unwrap());
                push_token(&mut tokens, &mut token, row, col);
                col += 1;
                if escape {
                    col += 1;
                }
            }

            '\"' => {
                push_token(&mut tokens, &mut token, row, col);
                let mut offset = 1;
                token.push(c);
                while let Some(c) = char_iter.next() {
                    token.push(c);
                    offset += 1;
                    if c == '\\' {
                        token.push(char_iter.next().unwrap());
                        offset += 1;
                        continue;
                    }
                    if c == '\"' {
                        break;
                    }
                }
                push_token(&mut tokens, &mut token, row, col);
                col += offset;
            }

            // if it hasnt been handled yet, add it to the token as it is either a number or a variable
            // todo: find a way to handle initial column position on iterations
            _ => {
                col += 1;
                token.push(c);
            }
        }
    }

    // Add the last token if it's not empty
    if !token.is_empty() {
        tokens.push(Token { token, row, col });
    }

    tokens
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_lex() {
        let code = "let x = 65 + 56;";
        let tokens = lex(code);
        assert_eq!(tokens.len(), 7);
        assert_eq!(tokens[0].token, "let");
        assert_eq!(tokens[1].token, "x");
        assert_eq!(tokens[2].token, "=");
        assert_eq!(tokens[3].token, "65");
        assert_eq!(tokens[4].token, "+");
        assert_eq!(tokens[5].token, "56");
        assert_eq!(tokens[6].token, ";");
    }

    #[test]
    fn test_lex_boolean_operators() {
        let code = ">====";
        let tokens = lex(code);
        assert_eq!(tokens.len(), 3);
        assert_eq!(tokens[0].token, ">=");
        assert_eq!(tokens[1].token, "==");
        assert_eq!(tokens[2].token, "=");
    }

    // #[test]
    // fn test_lex_token_location() {
    //     let code = "\n 56;";
    //     let tokens = lex(code);
    //     assert_eq!(tokens.len(), 2);
    //     assert_eq!(tokens[0].row, 2);
    //     assert_eq!(tokens[0].col, 2);
    //     assert_eq!(tokens[1].row, 4);
    // }
}
