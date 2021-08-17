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
	mut previus_item:=""
	mut is_tab:=true
	mut tab:=0.0
	mut is_func_def:= false
	mut is_argument:= false
	mut is_ccode:=false
	mut code_block:=Body{}
	mut json:=Ast{}
	for index,item in code{
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
		if item=="Ccode" && is_ccode==false && index!=0{
			is_ccode=true
		}
		else if is_ccode==true{
			code_block,is_ccode=ccode(item, is_ccode,tab)
		}
		else if item=="def"{
			previus_item=item
			is_func_def=true
		}
		else if is_func_def==true{
			code_block,previus_item,is_argument=function(item,is_func_def,previus_item,json,is_argument,tab)
		}
		if code_block!=Body{} && is_func_def==true && code_block.ast_type =="function_define" {
		json.body<<code_block
		code_block=Body{}
		}
		else if code_block!=Body{} && is_argument==true && is_func_def==true{
		json.body[json.body.len-1].right<<code_block
		code_block=Body{}
		}
		else if code_block!=Body{}  && is_argument==false && is_func_def==true{
		json.body[json.body.len-1].left<<code_block
		code_block=Body{}
		}

	}
	return json
}