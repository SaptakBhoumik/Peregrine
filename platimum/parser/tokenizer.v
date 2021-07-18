
pub fn enumerate(code string) []string {
	return code.split("")
} 

pub fn tokenize(str string) []string {
	mut code:=str.replace('    ','<tab>')
	white_space := ' '
	//defining an empty array
	mut result := []string{}
	mut lexeme := ''
	for i,char in enumerate(code){
		if char != white_space{
			lexeme += char
		}
	if i+1 < code.len(){
		if code[i+1] == white_space || code[i+1] in collection_of_keywords  || lexeme in collection_of_keywords{
			if lexeme != ''{
				lexeme  =lexeme.replace('\n', '<newline>')
				println(lexeme)
				result << lexeme
				lexeme = ''
			}
		}
	}
	}
	return result
}

symbols := [
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
				'=>',//greater than equals to
				'=<',//less than equals to
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
						'<newline>',//used to show \n
						'<tab>',//used to show tab
						'PRINT',
						'INPUT',
						'SYSTEM'//run command prompt commands
						]
//defining an empty array
pub mut collection_of_keywords := []string{}
//appending the keywords
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

code:=r"for i,char in enumerate(string):
    if char != white_space:
        lexeme += char # adding a char each time
    if (i+1 < len(string)): # prevents error
        if string[i+1] == white_space or string[i+1] in KEYWORDS or lexeme in KEYWORDS: # if next char == ' '
            if lexeme != '':
                print(lexeme.replace('\n', '<newline>'))
                lexeme = ''"