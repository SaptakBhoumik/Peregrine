module parser
// Original author: Saptak Bhoumik

pub struct Body {
	pub mut:
	ast_type string
	keyword string
	length int
	tab f64
	relative_to int
	direction string
	id int
	line int
}
struct Var{
	 pub mut:
	 variable string
	 var_type string
 }
struct Function{
	pub mut:
	name string
	variable []string
	variable_type []Var
	return_type []string
	free []string
}

pub struct Ast {
	pub mut:
	import_file []string
	header_file []string
	c_file []string
	folder string
	path string
	function_return_type []Function
	function_define []string=["__asm__","printf"]
	function_call []string=["main"]
	method_define []string
	method_call []string
	free []string
	body []Body
}

pub fn parser(code []string) (Ast,string){
	swallow_type:=["int","bool","str","list","dictionary","float","void"]
	required_arg:=["str_variable_required_argument","int_variable_required_argument","bool_variable_required_argument","list_variable_required_argument","dictionary_variable_required_argument","float_variable_required_argument","void_variable_required_argument"]
	decorator:=["@method","@free"]//more will be added soon
	operater:=["=","==",'+','-','*','/','^','//','%','>','<','>=','<=','!=']
	loop:=["if","while","elif","else","for"]
	logic:=["and","or","not","in","is"]
	error_handler:=["try","except","finally"]
	variable:="_variable"
	underscore:="_"
	mut var:=Var{}
	mut reserved_keywords:=[]string{}
	mut error := ""
	mut line:=0
	mut return_type:=false
	mut first_bracket_count:=0
	mut is_function_call:=false
	mut is_return:=false
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
	mut last_left_code_block:=Body{}
	mut json:=Ast{}
	reserved_keywords<<loop
	reserved_keywords<<swallow_type
	reserved_keywords<<decorator
	reserved_keywords<<operater
	reserved_keywords<<logic
	reserved_keywords<<error_handler
	reserved_keywords<<["Ccode","break","continue","return"]
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
			line++
			is_tab=true
		}
		if item==r"\n" && next_item==r"\n" && index!=code.len-1{
			is_tab==false
		}
		
		//parsing starts here
		if previous_code_block.keyword in swallow_type && item==":"{
					//do nothing
		}
		else if item==")" && is_function_call==true{
			code_block=Body{ast_type:"bracket"
							keyword : item
							length :item.len
							tab : tab}
		}
		else if item=="(" && previus_item==")" && (previous_code_block.ast_type in required_arg || previous_code_block.ast_type=="function_define"){
			return_type=true
		}
		
		else if return_type==true && item == ")"{
			return_type=false
		}
		else if return_type==true && is_function_call==false && item!="(" && (previous_code_block.ast_type=="function_define" || previous_code_block.ast_type in required_arg) && is_argument==false && is_operator==false && is_loop==false && is_ccode==false{
			if item in swallow_type{
				json.function_return_type[json.function_return_type.len-1].return_type<<item
			}
		}
		else if is_argument==true && item==")" && is_function_call==false && is_operator==false && is_loop==false && is_ccode==false{
			is_argument=false
			previus_item=item
		}
		else if is_argument==true && item!="("  && item!=r"\n" && item!=")" && item!="," && item!=" " && (item in reserved_keywords)==false && next_item!="(" && is_function_call==false  && (next_item in reserved_keywords)==false  && is_operator==false && is_loop==false && is_ccode==false{
				previus_item=item
				code_block=Body{ast_type:"required_argument"
							keyword : item
							length :item.len
							tab : tab
							direction : "right"
							id : index}
				if json.body.last().direction=="left"{
					code_block.relative_to=json.body.last().id
				}
				else{
					code_block.relative_to=json.body.last().relative_to
				}
				if previous_code_block.keyword in swallow_type || previous_code_block.keyword=="const"{
					code_block.ast_type="$previous_code_block.ast_type$underscore$code_block.ast_type"
				}
				json.body<<code_block
				code_block=Body{}
			}
		
		//checks if function call
		else if next_item=="."{
			code_block=Body{ast_type:know_type(item)
							keyword : item
							length :item.len
							tab : tab}
		}
		else if item=="." && next_item in json.method_define{
			if json.body[json.body.len-1].ast_type=="undefined"{
				json.body[json.body.len-1].ast_type="variable"
			}
			code_block=Body{ast_type:"dot"
							keyword : item
							length :item.len
							tab : tab}
		}
		else if previous_code_block.keyword=="." && item in json.method_define && next_item=="("{
			code_block=Body{ast_type:"method"
							keyword : item
							length :item.len
							tab : tab}
			is_function_call=true
		}
		else if know_type(item)=='undefined' && previus_item!="def" && next_item=="(" && (item in reserved_keywords)==false{
			code_block=Body{ast_type:"function_call"
							keyword : item
							length :item.len
							tab : tab}
			is_function_call=true
		}
		else if previous_code_block.ast_type=="function_call" && item=="(" && is_function_call==false{
			code_block=Body{ast_type:"bracket"
							keyword : item
							length :item.len
							tab : tab}
		}
		else if previous_code_block.ast_type=="method" && item=="(" && is_function_call==false{
			code_block=Body{ast_type:"bracket"
							keyword : item
							length :item.len
							tab : tab}
		}
		else if item in decorator{
			if code.len==index+3{
				error="Unexpected end of file"
				break
			}
			else{
				if item=="@method"{
					json.method_define<<code[index+3]
				}
				else if item=="@free"{
					json.free<<code[index+3]
				}
			}
		}
		else if item in swallow_type{
			if previous_code_block.keyword=="const"{
				code_block=Body{ast_type:"$item$underscore$previous_code_block.ast_type"
							keyword : item
							length :item.len
							tab : tab
							}
			}
			else{
				code_block=Body{ast_type:"$item$variable"
							keyword : item
							length :item.len
							tab : tab
							}
			}
		}
		else if item=="const"{
			code_block=Body{ast_type:"constant"
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
		//checks if raw string or formatted string
		else if item=="r"{
			code_block=Body{ast_type:"raw_string"
							keyword : item
							length :item.len
							tab : tab}
		}
		else if item=="f"{
			code_block=Body{ast_type:"formatted_string"
							keyword : item
							length :item.len
							tab : tab}
		}
		//checks if c code
		else if item=="Ccode" && is_ccode==false && index!=0{
			is_ccode=true
		}
		else if is_ccode==true{
			code_block,is_ccode=ccode(item, is_ccode,tab)
		}
		//checks if operator
		else if next_item in operater{
			is_operator=true
			code_block,is_operator=parse_operator(is_operator,item,tab,is_argument)
			if code_block.ast_type=='undefined' && next_item!="(" && previous_code_block.keyword=='const'{
				code_block.ast_type=previous_code_block.ast_type
			}
			else if code_block.ast_type=='undefined' && next_item!="(" && previous_code_block.keyword!='const'{
				code_block.ast_type="variable"
			}
		}
		else if item in operater{
			is_operator=true
			code_block,is_operator=parse_operator(is_operator,item,tab,is_argument)
		}
		else if is_operator==true && previous_code_block.ast_type!="function_call"{
			if is_argument==false{
				code_block,is_operator=parse_operator(is_operator,item,tab,is_argument)
				if code_block.ast_type=='undefined' && next_item!="("{
					code_block.ast_type="variable"
				}
			}
			else if is_argument==true && item!=")"{
				code_block,is_operator=parse_operator(is_operator,item,tab,is_argument)
				if code_block.ast_type=='undefined' && next_item!="("{
					code_block.ast_type="variable"
				}
			}
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
		else if is_loop==true && know_type(item)!="undefined" && next_item!="("{
			if item==":"{
				code_block=Body{}
			}
			else{
				code_block=Body{ast_type:know_type(item)
							keyword : item
							length :item.len
							tab : tab}
				if code_block.ast_type=='undefined' && next_item!="("{
					code_block.ast_type="variable"
				}
			}
		}
		else if is_loop==true && know_type(item)=="undefined" && next_item!="("{
			if item==":"{
				code_block=Body{}
			}
			else{
				code_block=Body{ast_type:know_type(item)
							keyword : item
							length :item.len
							tab : tab}
				if code_block.ast_type=='undefined' && next_item!="("{
					code_block.ast_type="variable"
				}
			}
		}
		
		else if is_function_call==true{
			code_block=Body{ast_type:know_type(item)
							keyword : item
							length :item.len
							tab : tab}

			if code_block.ast_type=='undefined'{
				code_block.ast_type="variable"
			}
		}

		//checks if function
		else if item=="def"{
			previus_item=item
			is_func_def=true
		}
		//checks if return statement
		else if item=="return"{
			is_return=true
			code_block=Body{ast_type:"return"
							keyword : item
							length :item.len
							tab : tab}
		}
		else if item!=r"\n" && is_return==true{
			code_block=Body{ast_type:know_type(item)
							keyword : item
							length :item.len
							tab : tab}
			if code_block.ast_type=='undefined' && next_item!="("{
				code_block.ast_type="variable"
			}
		}
		else if next_item=="," && is_argument==false{
			code_block=Body{ast_type:"variable"
							keyword : item
							length :item.len
							tab : tab}
		}
		else if item=="," && is_argument==false{
			code_block=Body{ast_type:"comma"
							keyword : item
							length :item.len
							tab : tab}
		}
		else if is_func_def==true  && item!=" " && is_function_call==false{
			
			if item!=","{
				code_block,previus_item,right=function(item,is_func_def,previus_item,json,right,tab)
				is_argument=right
			}
		}

		//checks if new line
		else if item==r"\n"{
			code_block=Body{ast_type:"new_line"
					keyword : item
					length :item.len
					tab : tab
					}
		}
		else{
			error="$item\n^Undefined character"
		}
		code_block.id=index
		code_block.line=line

		if item=="("{
			if previous_code_block.ast_type=="function_call" || previous_code_block.ast_type=="method"{
				first_bracket_count++
			}
		}
		else if is_function_call==true && item=="("{
			first_bracket_count++
		}
		else if is_function_call==true && item==")"{
			first_bracket_count--
		}
		if first_bracket_count==0{
			if item==")" && is_function_call==true{
				is_function_call=false
			}
			else{
				right=false
				is_function_call=false
			}
		}
		else if first_bracket_count>0{
			right=true
			is_function_call=true
		}
		if is_loop==true && item==":"{
			is_loop=false
			right=is_loop
		}
		else if is_loop==true && item!=":" && item in loop{
			right=false
		}
		else if is_loop==true && item!=":" {
			right=true
		}
		if is_return==true && item==r"\n"{
			is_return=false
			right=is_return
		}
		else if is_return==true && item!=r"\n" && item!=r"return"{
			right=true
		}
		//modify the code block
		
		if code_block.tab==0 && code_block.ast_type!="function_define"{
			code_block.tab=tab
		}
		
		if code_block.keyword!="" && code_block.keyword!=r"\n"{
			if right==false{
				code_block.direction="left"
			}
			else{
				code_block.direction="right"
			}
		}
		if previous_code_block.keyword in swallow_type{
			code_block.ast_type=previous_code_block.ast_type
			if item in json.function_return_type[json.function_return_type.len-1].variable{
				error="$item \n ^ Redefinition of an existing variable"
				break
			}
			else{
				if json.body.len==0{
					error="$item \n No function found"
				}
				else{
					json.function_return_type[json.function_return_type.len-1].variable<<item
					var.variable=item
					var.var_type=previous_code_block.keyword
					json.function_return_type[json.function_return_type.len-1].variable_type<<var
				}
			}
		}
		else if code_block.keyword in swallow_type{
			//do nothing
		}
		else if (code_block.ast_type=="variable"||code_block.ast_type=="constant") && item!="const" && previous_code_block.keyword!="," && next_item=="=" && (item in json.function_return_type[json.function_return_type.len-1].variable)==false{
			if code.len<=index+2{
				error="Unexpected end of file"
				break
			}
			else{
				json.function_return_type[json.function_return_type.len-1].variable<<item
				if know_type(code[index+2])!="undefined"{
					var.variable=item
					if know_type(code[index+2])=="float" || know_type(code[index+2])=="dictionary" || know_type(code[index+2])=="list" || know_type(code[index+2])=="bool"{
						var.var_type=know_type(code[index+2])
					}
					else if know_type(code[index+2])=="void" || know_type(code[index+2])=="pointer"{
						var.var_type="void"
					} 
					else if know_type(code[index+2])=="int" || code[index+2]=="("{
						var.var_type="float"
					}
					else if know_type(code[index+2])=="string"{
						var.var_type="str"
					}
				}
				else if code.len<=index+3{
					if code[index+2] in json.function_return_type.last().variable{
						var.variable=item
						for variable_type_item in json.function_return_type.last().variable_type{
							if variable_type_item.variable==code[index+2]{
								var.var_type=variable_type_item.var_type
								break
							}
						}
					}
					else{
						error="${code[index+2]}\n^Undefined variable"
						break
					}
				}
				else if code[index+3]!="("{
					if code[index+2] in json.function_return_type.last().variable{
						var.variable=item
						for variable_type_item in json.function_return_type.last().variable_type{
							if variable_type_item.variable==code[index+2]{
								var.var_type=variable_type_item.var_type
								
								break
							}
						}
					}
					else{
						error="${code[index+2]}\n^Undefined variable"
						break
					}
				}
				else if code[index+3]=="("{
					if code[index+2] in json.function_define{
						var.variable=item
						for function_type_item in json.function_return_type{
							if function_type_item.name==code[index+2]{
								if code[index+2]=="("{
									var.var_type="float"
								}
								else if function_type_item.return_type==[]string{}{
									var.var_type="void"
								}
								else if function_type_item.return_type.len==1{
									if json.function_return_type.last().return_type==[]string{} && code[index+2] in json.free{
										json.function_return_type[json.function_return_type.len-1].free<<item
									}
									var.var_type=function_type_item.return_type[0]
								}
								
								break
							}
						}
					}
					else{
						error="${code[index+2]}\n^Undefined function"
						break
					}
				}
				else{
					error="Unable to determine type of $item"	
				}
				json.function_return_type[json.function_return_type.len-1].variable_type<<var
			}
			
			if previous_code_block.keyword=="const"{
				code_block.ast_type="$var.var_type$underscore$previous_code_block.ast_type"
			}
			else{
				code_block.ast_type="$var.var_type$variable"
			}
		}
		else if previous_code_block.keyword == "const"{
			if code_block.ast_type=="required_argument"{
				code_block.ast_type="$previous_code_block.ast_type$underscore$var.var_type"
			}
			else{
				code_block.ast_type=previous_code_block.ast_type
			}
		}
		if code_block.ast_type=="string" && previous_code_block.keyword=="r"{
			code_block.ast_type=previous_code_block.ast_type
		}
		else if code_block.ast_type=="string" && previous_code_block.keyword=="f"{
			code_block.ast_type=previous_code_block.ast_type
			if json.function_return_type[json.function_return_type.len-1].return_type==[]string{}{
							json.function_return_type[json.function_return_type.len-1].free<<item
						}
		}
		//appends to json
		if  code_block.keyword!=""{
			json,error=tab_handler(mut json,mut code_block,mut previous_code_block, right,last_left_code_block)
		}
		json,error=error_check(mut json,code_block)
		if error!=""{
			json=Ast{}
			break
		}
		if json.body.len!=0{
			if json.body.last().keyword!=r"\n"{
				if json.body.last().direction=="left"{
					last_left_code_block=json.body.last()
				}
				if previous_code_block.keyword in swallow_type && item==":"{
					//do nothing
				}
				else{
					if code_block.keyword=="f" || code_block.keyword=="r"{
						code_block.relative_to=previous_code_block.relative_to
						code_block.direction=previous_code_block.direction
						previous_code_block=code_block
					}
					else{
						previous_code_block=json.body.last()
					}
					if json.body.last().keyword=="const" || json.body.last().keyword in swallow_type  || json.body.last().keyword=="def"{
						json.body.pop()
					}
				}	
			}
		}
		var=Var{}
		code_block=Body{}
	}
	return json,error
}