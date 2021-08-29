// author: Ethan Olchik

module preprocessor

import tokenizer

pub fn preprocess(src string) string[] {
	src = tokenize(src)

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
			}
		}
		counter++
	}

	return src
}