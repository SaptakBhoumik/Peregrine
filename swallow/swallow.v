import os
import tokenizer
import parser
import preprocessor
// Original author: Saptak Bhoumik
fn main() {
    filename := "./example.sw"
    content := os.read_file(filename) or {
        panic("failed to read $filename")
    }
    mut a:=content
    println(tokenizer.process_tokens(tokenizer.tokenize(a)))
    k:=parser.parser(tokenizer.process_tokens(tokenizer.tokenize(a)))
    println(k)
    println(preprocessor.formatter(content))
    println(tokenizer.process_tokens(tokenizer.tokenize(preprocessor.formatter(content))))
    // println(content)
    // println(os.args)//gives the location do i can add the built i file location
}
