#![allow(dead_code)]
use regex::Regex;

pub fn lex(raw_code: &str) -> Vec<&str> {
    let re:Regex = Regex::new(r"(==|!=|<=|>=|/|::|\+|\-|\*|\|&|=|;|:|,|\{|\}|\(|\)|<|>|\[|\]|[a-zA-Z]+([a-zA-Z0-9])*|\d+)").unwrap();
    let tokens: Vec<&str> = re.find_iter(raw_code).map(|m| m.as_str()).collect();
    return tokens;
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn lexer_test() {
        assert_eq!(vec!["1", "+", "1", "==", "2",], lex("1 + 1 == 2"));
    }
}
