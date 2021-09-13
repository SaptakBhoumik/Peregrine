module codegen
// Original author: Saptak Bhoumik
import parser
fn func_return(name string, ast parser.Ast) string{
	required_list:=ast.function_return_type
	mut return_item:=""
	mut type_of_return:=[]string{}
	for item in required_list{
		if name == item.name{
			type_of_return=item.return_type
			break
		}
	}
	if type_of_return==[]string{} || type_of_return==["void"]{
		return_item="void * $name"
	}
	return return_item
}