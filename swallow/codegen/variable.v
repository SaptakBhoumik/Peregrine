// Original author: Saptak Bhoumik

module codegen

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