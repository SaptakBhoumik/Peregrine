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


struct Body {
	pub mut:
	ast_type string
	keyword string
	length int
	tab f64
	left []Body
	right []Body
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

pub fn parser(code []string) Ast{
	operater:=["=","==",'+','-','*','/','^','//','%','>','<','>=','<=','!=']
	mut replace_previous:=false
	mut is_operator:=false
	mut is_constant:=false
	mut next_item:="string"
	mut right:=false
	mut previus_item:=""
	mut is_tab:=true
	mut tab:=0.0
	mut is_func_def:= false
	mut is_argument:= false
	mut is_ccode:=false
	mut code_block:=Body{}
	mut previous_code_block:=Body{}
	mut json:=Ast{}
	for index,item in code{
		//finds next item
		if index<code.len-1 && index!=0{
			next_item=code[index+1]
		}
		//calculates tab length
		if item==" " && is_tab==true{
			tab+=0.25
		}
		else if item!=" " && is_tab==true{
			is_tab=false
		}
		else if item==r"\n"{
			tab=0
			is_tab=true
		}
		//parsing starts here
		//checks if operator
		if next_item in operater{
			previus_item=item
		}
		else if item in operater{
			is_operator=true
			code_block,is_operator,replace_previous=parse_operator(is_operator,previus_item,item,tab, is_constant,mut previous_code_block)
			previus_item=item
		}
		else if is_operator==true{
			code_block,is_operator,replace_previous=parse_operator(is_operator,previus_item,item,tab, is_constant,mut previous_code_block)
			previus_item=item
		}
		//checks if c code
		else if item=="Ccode" && is_ccode==false && index!=0{
			is_ccode=true
		}
		else if is_ccode==true{
			code_block,is_ccode=ccode(item, is_ccode,tab)
		}
		//some basic keyword
		else if item=="pass"{
			code_block=Body{ast_type:"pass"
							keyword : item
							length :item.len
							tab : tab}
		}
		else if item=="break"{
			code_block=Body{ast_type:"break"
							keyword : item
							length :item.len
							tab : tab}
		}
		else if item=="continue"{
			code_block=Body{ast_type:"continue"
							keyword : item
							length :item.len
							tab : tab}
		}
		//checks if function
		else if item=="def"{
			previus_item=item
			is_func_def=true
		}
		else if is_func_def==true{
			code_block,previus_item,is_argument=function(item,is_func_def,previus_item,json,is_argument,tab)
		}
		//appends to json
		if code_block!=Body{} && is_func_def==true && code_block.ast_type =="function_define" {
		json.body<<code_block
		}
		else if code_block!=Body{} && is_argument==true && is_func_def==true{
				json.body.last().right<<code_block
		}
		else if code_block!=Body{}  && is_argument==false{
		if code_block.tab<1{
			panic("$code_block.keyword \n    	 ^ IndentationError: expected an indented block")
		}
		else{
		json=tab_handler(mut json,mut code_block,right,replace_previous)
		}
		previous_code_block=code_block
		code_block=Body{}
	}

	}
	return json
}