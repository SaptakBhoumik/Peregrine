// author: Ethan Olchik

module preprocessor

import tokenizer

pub fn preprocess(str string) []string {
	src: []string = tokenizer.process_tokens(tokenizer.tokenize(str))
	
	mut counter := 0
	
	for token in src {
		match token {
			"def" {
				counter++
				mut name: string = src[counter]
				if name == "main" {
					break
				}
				modulename := "MODULENAME"
				name = f"${modulename}${name}"
				src[counter] = name
			}
		}
		counter++
	}

	return src
}
