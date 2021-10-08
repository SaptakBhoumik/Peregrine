module codegen

// Original author: Saptak Bhoumik
import parser

fn func_return(name string, ast parser.Ast) (string, []string, parser.Function) {
	required_list := ast.function_return_type
	mut return_item := ''
	mut function := parser.Function{}
	mut type_of_return := []string{}
	mut free := []string{}
	for item in required_list {
		if name == item.name {
			type_of_return = item.return_type
			free = item.free
			function = item
			break
		}
	}
	if type_of_return.len == 0 {
		if type_of_return == []string{} && name == 'main' {
			return_item = 'int $name ('
		} else {
			return_item = 'void $name ('
		}
	} else if type_of_return.len == 1 {
		if type_of_return == ['void'] {
			return_item = 'void $name ('
		} else if type_of_return == ['str'] {
			return_item = 'char * $name ('
		} else if type_of_return == ['int'] {
			return_item = 'int64_t $name ('
		} else if type_of_return == ['int32'] {
			return_item = 'int32_t $name ('
		} else if type_of_return == ['int16'] {
			return_item = 'int16_t $name ('
		} else if type_of_return == ['int8'] {
			return_item = 'int8_t $name ('
		} else if type_of_return == ['uint'] {
			return_item = 'uint64_t $name ('
		} else if type_of_return == ['uint32'] {
			return_item = 'uint32_t $name ('
		} else if type_of_return == ['uint16'] {
			return_item = 'uint16_t $name ('
		} else if type_of_return == ['uint8'] {
			return_item = 'uint8_t $name ('
		} else if type_of_return == ['float'] {
			return_item = 'double $name ('
		} else if type_of_return == ['float32'] {
			return_item = 'float $name ('
		} else if type_of_return == ['char'] {
			return_item = 'char $name ('
		}
	}
	return return_item, free, function
}

fn func_return_if_else(name string, ast parser.Ast) []string {
	required_list := ast.function_return_type
	mut type_of_return := []string{}
	for item in required_list {
		if name == item.name {
			type_of_return = item.return_type
			break
		}
	}
	return type_of_return
}

fn required_arg(item parser.Body) string {
	mut final := ''
	keyword := item.keyword
	split_type := item.ast_type.split('_')
	if 'int' in split_type {
		final = 'int64_t $keyword'
	} else if 'int32' in split_type {
		final = 'int32_t $keyword'
	} else if 'int16' in split_type {
		final = 'int16_t $keyword'
	} else if 'int8' in split_type {
		final = 'int8_t $keyword'
	} else if 'uint' in split_type {
		final = 'uint64_t $keyword'
	} else if 'uint32' in split_type {
		final = 'uint32_t $keyword'
	} else if 'uint16' in split_type {
		final = 'uint16_t $keyword'
	} else if 'uint8' in split_type {
		final = 'uint8_t $keyword'
	} else if 'str' in split_type {
		final = 'char* $keyword'
	} else if 'bool' in split_type {
		final = 'bool $keyword'
	} else if 'float' in split_type {
		final = 'double $keyword'
	} else if 'float32' in split_type {
		final = 'float $keyword'
	} else if 'char' in split_type {
		final = 'char $keyword'
	}
	if 'constant' in split_type {
		final = 'const $final'
	}
	return final
}
