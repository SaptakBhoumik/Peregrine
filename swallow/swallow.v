import os
import tokenizer
import parser
import codegen

// import preprocessor

// Original Author(print_help && CLI): Ethan Olchik
fn print_help() {
	println('Swallow Compiler (C) The Swallow Organisation, MPL-2.0')
	println('Usage: swallow [command] [options] [file] -o [output file]\n')
	println('Swallow Commands:')
	println('\tcompile          - compiles a given file')
	println('\thelp             - shows this command')
	println('\nSwallow Options:')
	println('\t--emit-ast        - dumps ast in console and exits.')
	println('\t--emit-c          - generates C code and exits (skips C compilation phase).')
	println('\t--o=<output file> - select the file to output binary code to.')
	println('\nExample:')
	println('\tswallow compile example.sw --o=example')
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
	builtin_sw := os.read_file('${folder}builtin.sw') or {
		eprintln('Could not read file: ${folder}builtin.sw')
		return
	}

	builtin_h := os.read_file('${folder}builtin.h') or {
		eprintln('Could not read file: ${folder}builtin.sw')
		return
	}

	mut total := '$builtin_sw\n$content'
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
		final_code = '//Generated using the swallow compiler \n $builtin_h \n $final_code'

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

		os.system('gcc -O3 ./temp.c -o $opt.output')

		if !opt.emitc {
			os.system('rm ./temp.c')
		}
	} else {
		print('\033[0;31m')
		print('$error\n') // Display error in red
		print('\033[0m')
	}
}

// Original author: Pranav Baburaj(ArgumentParserOptions, argument_parser, execute)
struct ArgumentParserOptions {
mut:
	command string
	input string
	parameters map[string]string
	previous_element string
	error bool
	is_help bool
}

fn argument_parser(arguments []string) ArgumentParserOptions {
	mut options := ArgumentParserOptions{}
	if arguments.len == 0 {
		options.is_help = true
		return options
	}
	for index, argument in arguments {
		if index == 0{
			options.command = argument
			continue
		}
		if index == 1 {
			options.input = argument
			continue
		}

		options.previous_element = argument
		is_valid_parameter := argument.starts_with("--")
		if !is_valid_parameter {
			println("Invalid parameter: ${argument}\nParameters should start with --")
			options.error = true
			return options
		}
		slices := argument.split("=")
		key, value := slices[0][2..], slices[1..].join("=")
		options.parameters[key] = value
	}
	return options
}

fn execute(results ArgumentParserOptions) {
	if results.is_help || results.error {
		print_help()
		return
	}
	if results.command == "compile" {
		filename := results.input
		if filename.len == 0 { return }
			
		mut outfile := filename.trim(".sw")
		if 'o' in results.parameters {
			outfile = results.parameters['o']
		}
		compile(CompilationOptions{
			filename: filename,
			emitc: 'emit-c' in results.parameters,
			emitast: 'emit-ast' in results.parameters,
			output: outfile
		})
	}
}

// Original author: Saptak Bhoumik
fn main() {
	results := argument_parser(os.args.clone()[1..])
	execute(results)
}
