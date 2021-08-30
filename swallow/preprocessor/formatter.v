// author: Ethan Olchik

module preprocessor

import tokenizer

pub fn formatter(src string) string {
	mut tokens:=tokenizer.process_tokens(tokenizer.tokenize(src))
	mut source:=""
	mut previous_item:=""
	mut module_name:="MODULE"


	for mut token in tokens {
		if previous_item=="def" && token!="main"{
			token="$module_name$token"
		}
		else if token==r"\n"{
			token="\n"
		}
		previous_item=token
		if token!=" " && token!="\n"{
			token="$token "
		}
		unsafe{
			source+=*token
		}
	}

	return source
}