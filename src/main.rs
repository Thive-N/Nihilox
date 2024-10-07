#![cfg_attr(coverage_nightly, feature(coverage_attribute))]
mod ast;
mod lexer;
#[cfg_attr(coverage_nightly, coverage(off))]
fn main() {
    let tokens = lexer::lex("let x = 65 + 56;");
    println!("{:#?}", tokens);
}
