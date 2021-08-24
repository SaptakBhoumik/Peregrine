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
	relative_to int
	direction string
	id int
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
	loop:=["if","while","elif","else","for"]
	logic:=["and","or","not","in","is"]
	error_handler:=["try","except","finally"]
	basic_keyword:=["PRINT","SYSTEM","INPUT"]
	mut is_basic:=false
	mut is_operator:=false
	mut is_argument:=false
	mut next_item:="string"
	mut right:=false
	mut is_loop:=false
	mut previus_item:=""
	mut is_tab:=true
	mut tab:=0.0
	mut is_func_def:= false
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
		if item=="const"{
			code_block=Body{ast_type:item
						keyword : item
						length :item.len
						tab : tab
						}
		}
		else if item in error_handler{
			code_block=Body{ast_type:item
						keyword : item
						length :item.len
						tab : tab
						}
		}
		else if is_basic==true{
			code_block,is_operator=parse_operator(is_operator,item,tab )
			if code_block.ast_type=='undefined' && next_item!="("{
				code_block.ast_type="variable"
			}
		}
		else if item in basic_keyword{
			code_block=Body{ast_type:item
						keyword : item
						length :item.len
						tab : tab
						}
			is_basic=true
		}
		else if item in loop{
			code_block=loop_parse(item,tab)
			is_loop=true
		}
		else if item in logic{
			code_block=Body{ast_type:item
						keyword : item
						length :item.len
						tab : tab
						}
			is_loop=true
		}
		//checks if raw string
		else if item=="r"{
			code_block=Body{ast_type:item
							keyword : item
							length :item.len
							tab : tab}
		}
		else if next_item in operater{
			is_operator=true
			code_block,is_operator=parse_operator(is_operator,item,tab)
			// if code_block.ast_type=="undefined" && next_item=="("{
			// 	continue
			// }
			if code_block.ast_type=='undefined' && next_item!="(" && previous_code_block.ast_type=='const'{
				code_block.ast_type="constant"
			}
			else if code_block.ast_type=='undefined' && next_item!="(" && previous_code_block.ast_type!='const'{
				code_block.ast_type="variable"
			}
		}
		else if is_operator==true && is_argument==false{
			code_block,is_operator=parse_operator(is_operator,item,tab )
			if code_block.ast_type=='undefined' && next_item!="("{
				code_block.ast_type="variable"
			}
		}
		else if is_operator==true && is_argument==true && item!=")"{
			code_block,is_operator=parse_operator(is_operator,item,tab)
			if code_block.ast_type=='undefined' && next_item!="("{
				code_block.ast_type="variable"
			}
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
			code_block,previus_item,right=function(item,is_func_def,previus_item,json,right,tab)
			is_argument=right
		}
		//checks if new line
		else if item==r"\n"{
			code_block=Body{ast_type:"new_line"
					keyword : item
					length :item.len
					tab : tab
					}
		}
		code_block.id=index
		//modify the code block
		if code_block.keyword!="" && code_block.keyword!=r"\n"{
			if right==false{
				code_block.direction="left"
			}
			else{
				code_block.direction="right"
			}
		}
		if code_block.ast_type=="string" && previous_code_block.ast_type=="r"{
			code_block.ast_type="raw_string"
		}
		//appends to json
		if  code_block.keyword!=""{
			json=tab_handler(mut json,mut code_block,mut previous_code_block, right)
		}
		if code_block.keyword!=r"\n" && code_block.keyword!=""{
			previous_code_block=code_block
		}
		if is_loop==true && item==":"{
			is_loop=false
			right=is_loop
		}
		else if is_loop==true && item!=":"{
			right=true
		}
		if is_basic==true && item==r"\n"{
			is_basic=false
			right=is_loop
		}
		else if is_basic==true && item!="\n"{
			right=true
		}
		code_block=Body{}
	}
	return json
}