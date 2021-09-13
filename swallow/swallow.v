import os
import tokenizer
import parser
import codegen
// import preprocessor
// Original author: Saptak Bhoumik
fn main() {
    arg:=os.args.clone()
    filename := arg[2]
    content := os.read_file(filename) ?
    mut path:=os.executable()
    path=path.replace(r"\","/")
    mut folder:=""
    split_path:=path.split("/")
    split_path.pop()
    for item in split_path{
        folder="${folder}${item}/"
    }
    builtin_sw:= os.read_file("${folder}builtin.sw")?
    builtin_h:= os.read_file("${folder}builtin.h")?
    mut total:="${builtin_sw}\n${content}"
    ast,error:=parser.parser(tokenizer.process_tokens(tokenizer.tokenize(total)))
    mut final_code:=""
    if error==""{
       codes:=codegen.codegen(ast)
       for code in codes{
           final_code="$final_code$code"
       }
    final_code="//Generated using the swallow compiler \n $builtin_h \n $final_code"
    mut x:=os.create("temp.c")?
    x.writeln(final_code)?
    x.close()
    os.system("gcc ./temp.c -o ${arg.last()}")
    os.system("rm ./temp.c")
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