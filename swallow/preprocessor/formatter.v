// author: Ethan Olchik

module preprocessor

import tokenizer

fn find_token(token string , function_list []string) bool{
	mut result:=false
	for item in function_list{
		if item==token{
			result=true
			break
		}
	}
	return result
}

pub fn formatter(src string) string {
	mut next_item:=""
	mut tokens:=tokenizer.process_tokens(tokenizer.tokenize(src))
	mut function_list:=[]string{}
	mut source:=""
	mut previous_item:=""
	mut module_name:="MODULE"
	mut count:=tokens.len

	for index,mut token in tokens {
		//finds next item
		if index<count-1 && index!=0{
			next_item=tokens[index+1]
		}
		if previous_item=="def" && token!="main"{
			function_list<<token
			token="$module_name$token"
		}
		else if token==r"\n"{
			token="\n"
		}

		else if find_token(token,function_list)==true{
			if previous_item!="." && next_item=="("{
				token="$module_name$token"
			}

		}
		previous_item=token
		if token!=" " && token!="\n"{
			token="$token "
		}
		
		previous_item=token
		source = '$source$token'
	}

	return source
}