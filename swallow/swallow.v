import os
import tokenizer
import parser
// Original author: Saptak Bhoumik
fn main() {
    filename := "/home/saptak/Desktop/swallow/python_implication/example.sw"
    content := os.read_file(filename) or {
        panic("failed to read $filename")
    }
    mut a:=content
    println(tokenizer.process_tokens(tokenizer.tokenize(a)))
    k:=parser.parser(tokenizer.process_tokens(tokenizer.tokenize(a)))
    println(k)
    // println(content)
    // println(os.args)//gives the location do i can add the built i file location
}
