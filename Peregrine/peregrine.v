import os
import tokenizer
import parser
import codegen

// import preprocessor

// Original Author(print_help && CLI): Ethan Olchik
fn print_help() {
	println('Peregrine Compiler (C) The Peregrine Organisation, MPL-2.0')
	println('Usage: Peregrine [command] [options] [file] -o [output file]\n')
	println('Peregrine Commands:')
	println('\tcompile          - compiles a given file')
	println('\thelp             - shows this command')
	println('\nPeregrine Options:')
	println('\t-emit-ast        - dumps ast in console and exits.')
	println('\t-emit-c          - generates C code and exits (skips C compilation phase).')
	println('\t-o <output file> - select the file to output binary code to.')
	println('\nExample:')
	println('\tPeregrine compile example.pe -o example')
}

// Original Author (CompilationOptions and compile): Shravan Asati
struct CompilationOptions {
	filename string
	emitc    bool
	emitast  bool
	output   string
}

fn compile(opt CompilationOptions) {
	content := os.read_file(opt.filename) or {
		eprintln('Could not read file: ' + opt.filename)
		return
	}

	mut path := os.executable()
	path = path.replace(r'\', '/')
	mut folder := ''
	split_path := path.split('/')
	split_path.pop()
	for item in split_path {
		folder = '$folder$item/'
	}
	builtin_pe := os.read_file('${folder}builtin.pe') or {
		eprintln('Could not read file: ${folder}builtin.pe')
		return
	}

	builtin_h := os.read_file('${folder}builtin.h') or {
		eprintln('Could not read file: ${folder}builtin.pe')
		return
	}

	mut total := '$builtin_pe\n$content'
	tokens := tokenizer.process_tokens(tokenizer.tokenize(total))

	ast, error := parser.parser(tokens)
	if opt.emitast && !opt.emitc {
		print(ast)
		return
	} else if opt.emitast && !opt.emitc {
		println(tokens)
		println(ast)
		return
	}

	// println(tokenizer.process_tokens(tokenizer.tokenize(total)))
	mut final_code := ''
	if error == '' {
		// print(ast)
		// println(ast.function_return_type)
		codes := codegen.codegen(ast)
		for code in codes {
			final_code = '$final_code$code'
		}
		final_code = '//Generated using the Peregrine compiler \n $builtin_h \n $final_code'

		mut x := os.create('temp.c') or {
			eprintln('Unable to write the file.')
			return
		}

		x.writeln(final_code) or {
			eprintln('Unable to write the file.')
			return
		}

		x.close()
		if opt.emitc && !opt.emitast {
			return
		} else if opt.emitast && opt.emitc {
			println(ast)
			return
		}

		os.system('gcc -O3 temp.c -o $opt.output')

		if !opt.emitc {
			os.system('rm temp.c')
		}
	} else {
		print('\033[0;31m')
		print('$error\n') // Display error in red
		print('\033[0m')
	}
}

// Original author: Saptak Bhoumik
fn main() {
	arg := os.args.clone()
	mut filename := ''
	mut emitc := false
	mut emitast := false
	mut help := false
	mut o := false
	mut previous := ''
	mut outfile := ''
	mut idx_ := 0
	mut ext:=''
	if '${os.user_os()}'=="windows"{
		ext = ".exe"
	}
	for idx, x in arg {
		idx_ = idx
		if idx == 0 {
			previous = x
			continue
		}
		if x == 'compile' {
			previous = x
			if arg.last() == arg[idx] {
				help = true
				break
			}
			filename = arg[idx + 1]
			continue
		} else if x == '-emit-c' {
			previous = x
			emitc = true
			continue
		} else if x == '-emit-ast' {
			previous = x
			emitast = true
			continue
		} else if x == '-emit-token' {
			previous = x
			return
		} else if x == '-o' {
			previous = x
			o = true
			continue
		} else if x == 'help' {
			previous = x
			help = true
			continue
		} else if x.contains('.pe') {
			previous = x
			filename = x
			continue
		} else {
			if previous == '-o' {
				outfile = x
				continue
			}
			help = true
		}
	}

	if !o {
		outfile = "${filename.trim('.pe')}$ext"
	}

	if help || idx_ <= 0 {
		print_help()
		return
	}

	compile(CompilationOptions{
		filename: filename
		emitc: emitc
		emitast: emitast
		output: outfile
	})
}
