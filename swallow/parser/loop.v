module parser
//Original author: Saptak Bhoumik
fn loop_parse(item string,tab f64) Body{
	mut code_block:=Body{ast_type:item
					keyword : item
					length :item.len
					tab : tab
					}
	return code_block
}