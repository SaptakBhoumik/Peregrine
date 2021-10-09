module lexer

// Original author: Saptak Bhoumik
fn is_number(list []string) Token_type {
	mut type_of_str := Token_type.unknown
	numbers := [
		'0',
		'1',
		'2',
		'3',
		'4',
		'5',
		'6',
		'7',
		'8',
		'9',
	]
	if list[0] in numbers {
		if '.' in list {
			type_of_str = Token_type.decimal
		} else {
			type_of_str = Token_type.str
		}
	} else {
		type_of_str = Token_type.unknown
	}
	return type_of_str
}

pub fn know_type(item string) Token_type {
	mut type_of_str := Token_type.unknown
	split_text := item.split('')
	count:=split_text.len
		if item == 'True' || item == 'False' {
			type_of_str =  Token_type.true_false
		}  
		else if split_text[count - 1] == "'" && split_text[0] == "'" {
			type_of_str = Token_type.str
		} else if item == 'NULL' {
			type_of_str = Token_type.null
		} else if split_text[0] == '&' {
			type_of_str = Token_type.pointer
		} else if split_text[count - 1] == '"' && split_text[0] == '"' {
			type_of_str = Token_type.str
		} else if split_text[0] == '{' && split_text[count - 1] == '}' {
			type_of_str = Token_type.dictionary
		} else if split_text[0] == '[' && split_text[count - 1] == ']' {
			type_of_str = Token_type.list
		} else {
			type_of_str = is_number(split_text)
		}
	return type_of_str
}
