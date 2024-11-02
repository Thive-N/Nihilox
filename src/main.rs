#![cfg_attr(coverage_nightly, feature(coverage_attribute))]
mod ast;
mod lexer;
mod parser;
#[cfg_attr(coverage_nightly, coverage(off))]
fn main() {
    let tokens = lexer::lex("x + (-1 + 5 + 6)");
    let mut tokenstream = tokens.iter().peekable();
    let ast = parser::parse_expression(&mut tokenstream);
    println!("{:#?}", ast);
}
