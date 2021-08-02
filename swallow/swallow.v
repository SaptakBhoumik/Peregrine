import os
import tokenizer
// Original author: Saptak Bhoumik
fn main() {
    filename := "/home/saptak/Desktop/swallow/python_implication/example.sw"
    content := os.read_file(filename) or {
        panic("failed to read $filename")
    }
    mut a:=content
    k:=tokenizer.tokenize(a)
    println(tokenizer.process_tokens(k))
    // println(content)
    // println(os.args)//gives the location do i can add the built i file location
}