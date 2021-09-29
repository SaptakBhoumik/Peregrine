import os
import tokenizer
import parser
import codegen

// import preprocessor

// Original Author(print_help && CLI): Ethan Olchik
fn print_help() {
    println("Swallow Compiler (C) The Swallow Organisation, MPL-2.0")
    println("Usage: swallow [command] [options] [file] -o [output file]\n")
    println("Swallow Commands:")
    println("\tcompile          - compiles a given file")
    println("\thelp             - shows this command")
    println("\nSwallow Options:")
    println("\t-emit-ast        - dumps ast in console and exits.")
    println("\t-emit-c          - generates C code and exits (skips C compilation phase).")
    println("\t-o <output file> - select the file to output binary code to.")
    println("\nExample:")
    println("\tswallow compile example.sw -o example")
}

// Original author: Saptak Bhoumik

fn main() {
    arg:=os.args.clone()
    mut filename := ""
    mut emitc := false
    mut emitast := false
    mut help := false
    mut o := false
    mut previous := ""
    mut outfile := ""
    mut idx_ := 0
    for idx, x in arg {
        idx_ = idx
        if idx == 0 {
            previous = x
            continue
        }
        if x == "compile" {
            previous = x
            if arg.last() == arg[idx] {
                help=true
                break
            }
            filename = arg[idx+1]
            continue
        }
        else if x == "-emit-c" {
            previous = x
            emitc = true
            continue
        }
        else if x == "-emit-ast" {
            previous = x
            emitast = true
            continue
        }
        else if x == "-emit-token" {
            previous = x
            return
        }
        else if x == "-o" {
            previous = x
            o = true
            continue
        }
        else if x == "help" {
            previous = x
            help=true
            continue
        }
        else if x.contains(".sw") {
            previous = x
            filename = x
            continue
        }
        else {
            if previous == "-o" {
                outfile = x
                continue
            }
            help = true
        }
    }

    if !o {
        outfile = filename.trim(".sw")
    }

    if help || idx_ <= 0 {
        print_help()
        return
    }

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
    tokens := tokenizer.process_tokens(tokenizer.tokenize(total))

    ast,error:=parser.parser(tokens)
    if emitast && !emitc {
        print(ast)
        return
    } else if emitast && !emitc {
        println(tokens)
        println(ast)
        return
    }

    // println(tokenizer.process_tokens(tokenizer.tokenize(total)))
    mut final_code:=""
    if error==""{
    // print(ast)
    // println(ast.function_return_type)
       codes:=codegen.codegen(ast)
       for code in codes{
           final_code="$final_code$code"
       }
    final_code="//Generated using the swallow compiler \n $builtin_h \n $final_code"
    mut x:=os.create("temp.c")?
    x.writeln(final_code)?
    x.close()
    if emitc && !emitast {
        return
    }
    else if emitast && emitc {
        println(ast)
        return
    }

    os.system("gcc ./temp.c -o ${outfile}")
    
    if !emitc {
        os.system("rm ./temp.c")
    }
    }
    else{
        print("\033[0;31m")
        print("$error\n")//Display error in red
        print("\033[0m")
    }
    // println(preprocessor.formatter(content))
    // println(tokenizer.process_tokens(tokenizer.tokenize(preprocessor.formatter(content))))
    // println(preprocessor.import_parser(content))
    // println(content)
    // println(os.args)//gives the location do i can add the built i file location
}