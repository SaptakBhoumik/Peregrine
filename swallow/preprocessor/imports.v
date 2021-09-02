// Original Author: Ethan Olchik

module preprocessor

import tokenizer
import parser

import os


pub fn process_imports(src string) Ast {
	tokens = tokenizer.process_tokens(tokenizer.tokenize(src))
	next_item = ""
	previous_item = ""
	rtks = ""
	ast := Ast {}
	for index, mut item in tokens {
		if index < count - 1 && index != 0 {
			next_item=tokens[index+1]
		}
		
		mut path = ""
		for item != r"\n" {
			path += item
		}

		path.replace(".", "/")
		path += match item {
			"import"  {  ".sw" }
			"cimport" {  ".c"  }
			"himport" {  ".h"  }
			else { continue }
		}

		buf = os.read_file(path)

		tks = tokenizer.process_tokens(tokenizer.tokenize(buf))

		ast = parser.paresr(tks)
	}

	return ast
}