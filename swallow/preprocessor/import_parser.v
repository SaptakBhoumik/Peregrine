module preprocessor
// author: Saptak Bhoumik
import tokenizer
struct Import_ast{
	pub mut:
	swallow_file []string
	header_file []string
	c_file []string
}
pub fn import_parser(code string) Import_ast{
	mut temp:=""
	mut ast:=Import_ast{}
	mut previous_item:=""
	mut is_swallow_file:=false
	mut is_header_file:=false
	mut is_c_file:=false
	mut tokens:=tokenizer.process_tokens(tokenizer.tokenize(code))
	for mut item in tokens{
		if item!=","{
			if item=="def"{
				break //when a function is being defined then there should be no import after it
			}
			else if item == "import"{
				is_swallow_file=true
			}
			else if item == "himport"{
				is_header_file=true
			}
			else if item == "cimport"{
				is_c_file=true
			}
			//for swallow files
			else if is_swallow_file==true{
				if item==r"\n"{
					is_swallow_file=false
				}
				else if previous_item=="import" || previous_item==","{
					ast.swallow_file<<item
				}
				else if ast.swallow_file.len !=0{
					temp=ast.swallow_file[ast.swallow_file.len-1]
					temp="$temp$item"
					ast.swallow_file[ast.swallow_file.len-1]=temp
				}
			}
			//for header files
			else if is_header_file==true{
				if item==r"\n"{
					is_header_file=false
				}
				else if previous_item=="himport" || previous_item==","{
					ast.header_file<<item
				}
				else if ast.header_file.len !=0{
					temp=ast.header_file[ast.header_file.len-1]
					temp="$temp$item"
					ast.header_file[ast.header_file.len-1]=temp
				}
			}
			//for c files
			else if is_c_file==true{
				if item==r"\n"{
					is_c_file=false
				}
				else if previous_item=="cimport" || previous_item==","{
					ast.c_file<<item
				}
				else if ast.c_file.len !=0{
					temp=ast.c_file[ast.c_file.len-1]
					temp="$temp$item"
					ast.c_file[ast.c_file.len-1]=temp
				}
			}
		}
		previous_item=item
	}
	return ast
}