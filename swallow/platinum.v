import os
import tokenizer
fn main() {
    filename := "/home/saptak/Desktop/platinum/python_implication/example.pt"
    content := os.read_file(filename) or {
        panic("failed to read $filename")
    }
    mut a:=content
    k:=tokenizer.tokenize(a)
    println(tokenizer.process_tokens(k))
    // println(content)
    // println(os.args)//gives the location do i can add the built i file location
}