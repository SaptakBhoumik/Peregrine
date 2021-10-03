module tokenizer

// Original author: Saptak Bhoumik
pub fn tokenize(str string) []string {
	// it partially tokenizes the code
	other_keywords := [
		'True',
		'False',
		'None',
	]
	symbols := [
		' ',
		/* white space */
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
		'#',
		/* comment */
		'`',
		// acute sign
	]
	logical_keywords := [
		'&',
		'|',
		'~',
		'=',
		/* equals */
		'+',
		/* addition */
		'-',
		/* subtraction */
		'*',
		/* multiplication */
		'/',
		/* division */
		'^',
		/* exponent */
		'//',
		/* quotient */
		'%',
		/* remainder */
		'>',
		/* greater than */
		'<',
		/* less than */
		'>=',
		/* greater than equals to */
		'<=',
		/* less than equals to */
		'!='
		// not equals to
	]

	operator_keywords := [
		'and',
		'or',
		'not',
		'in',
		'is',
	]
	conditional_keywords := [
		'if',
		'elif',
		'else',
	]
	iteration_keywords := [
		'for',
		'while',
		'break',
		'continue',
		'pass',
	]
	structure_keywords := [
		'def',
	]
	return_keywords := [
		'return',
	]
	import_keywords := [
		'import',
		'himport',
		'cimport',
	]
	exception_keywords := [
		'try',
		'except',
		'raise',
		'finally',
	]

	variable_keybords := [
		'int',
		'bool',
		'str',
		'list',
		'dictionary',
		'float',
		'void',
		'const',
	]

	// defining an empty array
	mut collection_of_keywords := []string{}

	// appending the keywords
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
	mut code := '\n$str  ' // do not change this line
	white_space := ' '

	// defining an empty array
	mut result := []string{}
	mut lexeme := ''
	for i, mut char in code.split('') {
		lexeme += '$char' // do not change this line
		mut a := i + 1
		if a < code.len {
			if code[a].ascii_str() == white_space || code[a].ascii_str() in collection_of_keywords
				|| lexeme in collection_of_keywords {
				if lexeme != '' {
					lexeme = lexeme.replace('\n', r'\n')
					result << lexeme
					lexeme = ''
				}
			}
		}
	}
	return result
}
