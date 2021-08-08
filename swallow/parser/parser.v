module parser
// Original author: Saptak Bhoumik

struct Know_type {
	mut:
	is_var bool
	name string
	check_type string
}

struct Function_return_type {
	mut:
	name string
	check_type string
}


struct Body {
	mut:
	column int
	ast_type string
	keyword string
	length string
}

struct Ast {
	mut:
	path string
	function_name []string
	function_return []Function_return_type
	variable_name []string
	variable_type []Know_type
	constant_name []string
	constant_type []Know_type
	body []Body
}

fn is_number(list []string)string {
	mut type_of_str:="undefined"
	numbers:=[
			'0',
			'1',
			'2',
			'3',
			'4',
			'5',
			'6',
			'7',
			'8',
			'9'
			]
	if list[0] in numbers{
		if "." in list{
			type_of_str = "float"
		}
		else{
			type_of_str = "int"
		}
	}
	else{
		type_of_str = "undefined"
	}
	return type_of_str
}
fn know_type(item string) string{
    mut type_of_str:="undefined"
	split_text:=item.split("")
	count:=split_text.len
	if count>0{
		if item=="True" || item=="False"{
			type_of_str="bool"
		}
		else if split_text[count-1]=="'" && split_text[0]=="'"{
			type_of_str="string"
		}
		else if split_text[count-1]=='"' && split_text[0]=='"'{
			type_of_str="string"
		}
		else if  split_text[0]=='{' && split_text[count-1]=='}'{
			type_of_str="dictionary"
		}
		else if  split_text[0]=='[' && split_text[count-1]==']'{
			type_of_str="list"
		}
		else{
			type_of_str=is_number(split_text)
		}
	}
	else{
		type_of_str="undefined"
	}
	return type_of_str
}

pub fn parser(code []string)string {
	mut json:=""
	return json
}