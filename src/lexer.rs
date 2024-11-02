#![allow(dead_code)]

use std::fmt::Display;
#[derive(Debug)]
pub struct Token {
    pub token: String,
    pub row: u32,
    pub col: u32,
}

struct LexerState<'a> {
    tokens: Vec<Token>,
    token: String,
    current_row: u32,
    current_col: u32,
    current_token_row: u32,
    current_token_col: u32,
    char_iter: std::iter::Peekable<std::str::Chars<'a>>,
}

impl<'a> LexerState<'a> {
    fn default(raw_code: &'a str) -> Self {
        LexerState {
            tokens: Vec::new(),
            token: String::new(),
            current_row: 1,
            current_col: 0,
            current_token_row: 1,
            current_token_col: 1,
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
            row: state.current_token_row,
            col: state.current_token_col,
        });
        state.token.clear();
    }
}

fn set_token_location(state: &mut LexerState) {
    state.current_token_row = state.current_row;
    state.current_token_col = state.current_col;
}

pub fn lex(raw_code: &str) -> Vec<Token> {
    let mut state = LexerState::default(raw_code);
    while let Some(c) = state.char_iter.next() {
        state.current_col += 1;
        match c {
            ' ' | '\t' | '\r' => {}

            '\n' => {
                state.current_row += 1;
                state.current_col = 0;
            }

            // boolean operators
            '<' | '>' | '=' | '!' => {
                set_token_location(&mut state);
                state.token.push(c);
                if state.char_iter.peek() == Some(&'=') {
                    state.char_iter.next();
                    state.token.push('=');
                    state.current_col += 1;
                }
                push_token(&mut state);
            }

            // single character tokens
            '+' | '-' | '*' | '/' | '%' | ':' | ';' | '(' | ')' => {
                set_token_location(&mut state);
                state.token.push(c);
                push_token(&mut state);
            }

            // handle char strings
            '\'' => {
                set_token_location(&mut state);
                state.token.push(c);
                // if the next character is a backslash, we need to escape it
                if state.char_iter.peek() == Some(&'\\') {
                    state.token.push(state.char_iter.next().unwrap()); // escape character
                    state.current_col += 1;
                }
                state.token.push(state.char_iter.next().unwrap()); // character
                state.token.push(state.char_iter.next().unwrap()); // closing quote
                state.current_col += 2;
                push_token(&mut state);
            }

            '\"' => {
                set_token_location(&mut state);
                state.token.push(c);
                while let Some(c) = state.char_iter.next() {
                    state.token.push(c);
                    state.current_col += 1;
                    if c == '\\' {
                        state.token.push(state.char_iter.next().unwrap());
                        state.current_col += 1;
                        continue;
                    }
                    if c == '\"' {
                        break;
                    }
                }
                push_token(&mut state);
            }

            _ => {
                set_token_location(&mut state);
                state.token.push(c);
                while let Some(c) = state.char_iter.peek() {
                    if c.is_alphanumeric() || *c == '_' {
                        state.token.push(state.char_iter.next().unwrap());
                        state.current_col += 1;
                    } else {
                        break;
                    }
                }
                push_token(&mut state);
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

    #[test]
    fn token_format_test() {
        let token = Token {
            token: "let".to_string(),
            row: 1,
            col: 1,
        };
        assert_eq!(format!("{}", token), "Token: let at row: 1, col: 1");
    }

    #[test]
    fn test_lex_token_location() {
        let code = "\n 56;";
        let tokens = lex(code);
        assert_eq!(tokens.len(), 2);
        assert_eq!(tokens[0].row, 2);
        assert_eq!(tokens[0].col, 2);
        assert_eq!(tokens[1].col, 4);
    }
}
