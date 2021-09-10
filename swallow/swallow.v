import os
import tokenizer
import parser
#include <stdio.h>
// import preprocessor
// Original author: Saptak Bhoumik
fn main() {
    filename := "./example.sw"
    content := os.read_file(filename) or {
        panic("failed to read $filename")
    }
    mut a:=content
    println(tokenizer.process_tokens(tokenizer.tokenize(a)))
    k,error:=parser.parser(tokenizer.process_tokens(tokenizer.tokenize(a)))
    if error==""{
       println(k)
    }
    else{
        print("\033[0;31m")
        print("$error\n")//Display Hello in red
        print("\033[0m")
    }
    // println(preprocessor.formatter(content))
    // println(tokenizer.process_tokens(tokenizer.tokenize(preprocessor.formatter(content))))
    // println(preprocessor.import_parser(content))
    // println(content)
    // println(os.args)//gives the location do i can add the built i file location
}
