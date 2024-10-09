#![allow(dead_code)]

use std::fmt::Display;
#[derive(Debug)]
pub struct Token {
    token: String,
    row: u32,
    col: u32,
}

struct LexerState<'a> {
    tokens: Vec<Token>,
    token: String,
    row: u32,
    col: u32,
    char_iter: std::iter::Peekable<std::str::Chars<'a>>,
}

impl<'a> LexerState<'a> {
    fn default(raw_code: &'a str) -> Self {
        LexerState {
            tokens: Vec::new(),
            token: String::new(),
            row: 1,
            col: 0,
            char_iter: raw_code.chars().peekable(),
        }
    }
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

fn push_token(state: &mut LexerState) {
    if !state.token.is_empty() {
        state.tokens.push(Token {
            token: state.token.clone(),
            row: state.row,
            col: state.col,
        });
        state.token.clear();
    }
}

pub fn lex(raw_code: &str) -> Vec<Token> {
    let mut state = LexerState::default(raw_code);
    while let Some(c) = state.char_iter.next() {
        match c {
            ' ' | '\t' | '\r' => {
                push_token(&mut state);
                state.col += 1;
            }

            '\n' => {
                push_token(&mut state);
                state.row += 1;
                state.col = 0;
            }

            // boolean operators
            '<' | '>' | '=' | '!' => {
                push_token(&mut state);
                state.col += 1;
                state.token.push(c);
                if state.char_iter.peek() == Some(&'=') {
                    state.char_iter.next();
                    state.token.push('=');
                    push_token(&mut state);
                    state.col += 1;
                } else {
                    push_token(&mut state);
                }
            }

            // single character tokens
            '+' | '-' | '*' | '/' | '%' | ':' | ';' => {
                push_token(&mut state);
                state.col += 1;
                state.token.push(c);
                push_token(&mut state);
                state.col += 1;
            }

            // handle char strings
            '\'' => {
                let mut escape = false;
                push_token(&mut state);
                state.token.push(c);
                // if the next character is a backslash, we need to escape it
                if state.char_iter.peek() == Some(&'\\') {
                    escape = true;
                    state.token.push(state.char_iter.next().unwrap());
                    state.token.push(state.char_iter.next().unwrap());
                } else {
                    state.token.push(state.char_iter.next().unwrap());
                }
                state.token.push(state.char_iter.next().unwrap());
                push_token(&mut state);
                state.col += 1;
                if escape {
                    state.col += 1;
                }
            }

            '\"' => {
                push_token(&mut state);
                let mut offset = 1;
                state.token.push(c);

                while let Some(c) = state.char_iter.next() {
                    state.token.push(c);
                    offset += 1;
                    if c == '\\' {
                        state.token.push(state.char_iter.next().unwrap());
                        offset += 1;
                        continue;
                    }
                    if c == '\"' {
                        break;
                    }
                }
                push_token(&mut state);
                state.col += offset;
            }

            // if it hasnt been handled yet, add it to the token as it is either a number or a variable
            // todo: find a way to handle initial column position on iterations
            _ => {
                state.col += 1;
                state.token.push(c);
            }
        }
    }

    push_token(&mut state);

    state.tokens
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
