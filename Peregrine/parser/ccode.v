module parser

// Original author: Saptak Bhoumik
pub fn ccode(item string, ccode bool, tab_len f64) (Body, bool) {
	mut code_block := Body{}
	mut is_ccode := ccode
	if item != 'Ccode' && is_ccode == true {
		code_block = Body{
			ast_type: 'Ccode'
			keyword: item
			length: item.len
			tab: tab_len
		}
	} else if item == 'Ccode' && is_ccode == true {
		is_ccode = false
	}
	return code_block, is_ccode
}
