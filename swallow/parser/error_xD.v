module parser
// Original author: Saptak Bhoumik

fn error_check(mut json Ast,code_block Body) (Ast,string){
	mut error:=""
	if code_block.ast_type=="function_call"{
		if code_block.keyword in json.function_define{
			json.function_call<<code_block.keyword
		}
		else{
			error="$code_block.keyword\n^Undefined function"
		}
	}
	else if code_block.ast_type=="function_define"{
		if code_block.keyword in json.function_define{
			error="$code_block.keyword\n^Redefination of an existing function"
		}
		else{
			json.function_define<<code_block.keyword
		}
	}
	return json,error
}