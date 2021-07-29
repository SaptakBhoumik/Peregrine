module tokenizer

fn remove_comment(str string) string{
	code := "$str \n"//dont change this line
	mut modified_code:=""
	mut lines := code.split_into_lines()
	for mut line in lines{
		mut modified_line:=line.replace(" ","")
		modified_line=modified_line.replace("	","")
		if modified_line.len > 0 {
		mut first_char:=modified_line[0].ascii_str()
		if first_char!="#"{
			modified_code += "$line \n"
	}}}
	return modified_code
}

pub fn tokenize(str string) []string {
	other_keywords := [
					'True',
					'False',
					'None'
					]
	symbols := [
			' ',//white space
			'!',
			'{',
			'}',
			'(',
			')',
			'[', 
			']', 
			'.', 
			'"', 
			"'",
			'\n', 
			':', 
			',',
			'#'//comment
			] 
	logical_keywords :=[
					'=',//equals
					'+',//addition
					'-',//subtraction
					'*',//multiplication
					'/',//division
					'^',//exponent
					'//',//quotient
					'%',//remainder
					'>',//greater than
					'<',//less than
					'>=',//greater than equals to
					'<=',//less than equals to
					'!='//not equals to
					]

	operator_keywords := [
				'and', 
				'or', 
				'not', 
				'in', 
				'is'
				]
	conditional_keywords :=[
							'if',
							'elif',
							'else'
							]
	iteration_keywords :=[
							'for',
							'while',
							'break',
							'continue',
							'pass'
							]
	structure_keywords :=[
							'def',
							'as',
							'with'
							]
	return_keywords := [
						'return',
						'yield'
						]
	import_keywords :=[
						'import'
						]
	exception_keywords := [
							"try", 
							"except", 
							"raise", 
							"finally"
							]

	variable_keybords := [
							"global",
							"int",
							"str",
							"float",
							"cons",
							"var"
							]
	functional_keywords := [
							'PRINT',
							'INPUT',
							'SYSTEM'//run command prompt commands
							]
	//defining an empty array
	mut collection_of_keywords := []string{}
	//appending the keywords
	collection_of_keywords << other_keywords
	collection_of_keywords << symbols
	collection_of_keywords << logical_keywords
	collection_of_keywords << operator_keywords
	collection_of_keywords << conditional_keywords
	collection_of_keywords << iteration_keywords
	collection_of_keywords << structure_keywords
	collection_of_keywords << return_keywords
	collection_of_keywords << import_keywords
	collection_of_keywords << exception_keywords
	collection_of_keywords << variable_keybords 
	collection_of_keywords << functional_keywords
	mut code:=remove_comment("$str " )//do not change this line
	white_space := ' '
	//defining an empty array
	mut result := []string{}
	mut lexeme := ''
	for i,mut char in code.split(""){	
		lexeme += "$char" //do not change this line
		mut a:=i+1
		if a < code.len{
			if code[a].ascii_str() == white_space || code[a].ascii_str() in collection_of_keywords  || lexeme in collection_of_keywords{
				if lexeme != ''{
					lexeme  =lexeme.replace('\n', r'\n')
					result << lexeme
					lexeme = ''
		}}}}
	return result
}