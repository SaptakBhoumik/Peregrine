// Original author: Saptak Bhoumik

module codegen
import parser
fn variable(ast_type string,keyword string)(string){
	mut final:=""
	if ast_type=="int_variable"{
		final="int64_t $keyword"
	}
	else if ast_type=="str_variable"{
		final="char* $keyword"
	}
	else if ast_type=="bool_variable"{
		final="bool $keyword"
	}
	else if ast_type=="float_variable"{
		final="double $keyword"
	}
	return final
}
fn var_type(var []parser.Var,keyword string)(string){
	mut result:=""
	for item in var{
		if keyword==item.variable{
			result=item.var_type
			break
		}
	}
	return result
}