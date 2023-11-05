#![cfg_attr(coverage_nightly, feature(coverage_attribute))]
mod lexer;

#[cfg_attr(coverage_nightly, coverage(off))]
fn main() {
    println!("Hello, world!");
}
