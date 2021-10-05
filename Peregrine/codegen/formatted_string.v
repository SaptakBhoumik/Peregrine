// Original Author: Ethan Olchik
module codegen

import parser

fn fstring(item parser.Body) string {
	mut code := ''
	mut args := []string{}
	/*@param item:
	*	- 'variable'
	* @return:
	*/

	code += '_format(${item.keyword.all_before('{')}{}${item.keyword.find_between('}',
		'{').trim('"')}'
	args << item.keyword.find_between('{', '}')
	mut x := item.keyword.all_after('}')
	for x.count('{') > 0 {
		code += '{}${x.find_between('}', '{').trim('"')}'
		args << x.find_between('{', '}')
		x = x.all_after('}')
	}

	code += '",'

	for arg in args {
		code += ' $arg,'
	}

	code = code.all_before_last(',')

	code += ')'

	return code
}
