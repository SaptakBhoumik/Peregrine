module lexer

// Original author: Saptak Bhoumik
fn is_number(list []string) string {
	mut type_of_str := 'undefined'
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
			type_of_str = 'FLOAT'
		} else {
			type_of_str = 'INT'
		}
	} else {
		type_of_str = 'UNKNOWN'
	}
	return type_of_str
}

pub fn know_type(item string) string {
	mut type_of_str := 'UNKNOWN'
	split_text := item.split('')
	count:=split_text.len
		if item == 'True' || item == 'False' {
			type_of_str = 'BOOL'
		}  
		else if split_text[count - 1] == "'" && split_text[0] == "'" {
			type_of_str = 'STR'
		} else if item == 'NULL' {
			type_of_str = 'VOID'
		} else if split_text[0] == '&' {
			type_of_str = 'POINTER'
		} else if split_text[count - 1] == '"' && split_text[0] == '"' {
			type_of_str = 'STR'
		} else if split_text[0] == '{' && split_text[count - 1] == '}' {
			type_of_str = 'DICTIONARY'
		} else if split_text[0] == '[' && split_text[count - 1] == ']' {
			type_of_str = 'LIST'
		} else {
			type_of_str = is_number(split_text)
		}
	return type_of_str
}
