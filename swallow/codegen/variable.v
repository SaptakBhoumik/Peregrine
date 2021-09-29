// Original author: Saptak Bhoumik

module codegen
import parser
fn variable(ast_type string,keyword string)(string){
	mut final:=""
	split_type:=ast_type.split("_")
	if "int" in split_type{
		final="int64_t $keyword"
	}
	else if "int32" in split_type{
		final="int32_t $keyword"
	}
	else if "int16" in split_type{
		final="int16_t $keyword"
	}
	else if "int8" in split_type{
		final="int8_t $keyword"
	}
	else if "str" in split_type{
		final="char* $keyword"
	}
	else if "char" in split_type{
		final="char $keyword"
	}
	else if "bool" in split_type{
		final="bool $keyword"
	}
	else if "float" in split_type{
		final="double $keyword"
	}
	else if "float32" in split_type{
		final="float $keyword"
	}
	else if "void" in split_type{
		final="void $keyword"
	}
	if "constant" in split_type{
		final="const $final"
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