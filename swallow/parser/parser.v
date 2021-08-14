module parser
// Original author: Saptak Bhoumik

struct Know_type {
	pub mut:
	is_var bool
	is_static bool
	name string
	check_type string
}

struct Function_return_type {
	pub mut:
	name string
	check_type string
}

struct Right{
	pub mut:
	ast_type string
	keyword string
	length int
	left []&Left
	right []&Right
}

struct Left{
	pub mut:
	ast_type string
	keyword string
	length int
	left []&Left
	right []&Right
}

struct Body {
	pub mut:
	ast_type string
	keyword string
	length int
	left []&Left
	right []&Right
}

struct Ast {
	pub mut:
	import_file []string
	header_file []string
	c_file []string
	folder string
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

pub fn parser(code []string) Ast{
	mut is_ccode:=false
	mut code_block:=Body{}
	mut json:=Ast{}
	for index,item in code{
		if item=="Ccode" && is_ccode==false{
			is_ccode=true
		}
		else if item!="Ccode" && is_ccode==true{
			code_block=Body{ast_type:"Ccode"
							keyword : item
							length :item.len}
		}
		else if  item=="Ccode" && is_ccode==true{
			is_ccode=false
		}
		if code_block!=Body{}{
		json.body<<code_block
		code_block=Body{}
		}
	}
	return json
}