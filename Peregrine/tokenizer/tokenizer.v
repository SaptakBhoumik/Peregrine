module tokenizer

// Original author: Saptak Bhoumik
pub fn tokenize(str string) []string {
	arithmetic_operators := [
		'+',
		'-',
		'*',
		'/',
		'%',
		'**',
		'//',
	]
	
	assignment_operators := [
		'=',
		'+=',
		'-=',
		'*=',
		'/=',
		'%=',
		'//=',
		'**=',
		'&=',
		'|=',
		'^=',
		'>>=',
		'<<=',
	]
	
	comparison_operators := [
		'==',
		'!=',
		'>',
		'<',
		'>=',
		'<=',
		'is',
		'in',
	]
		
	logical_operators := [
		'and',
		'or',
		'not',
	]
	
	identity_operators := [
		'is',
		'is not',
	]
	
	membership_operators := [
		'in',
		'not in',
	]
	
	bitwise_operators := [
		'&',
		'|',
		'^',
		'~',
		'<<',
		'>>',
	]
	
	value_keywords := [
		'True',
		'False',
		'None',
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
	]
	
	structure_keywords := [
		'def',
		'class',
		'with',
		'as',
		'pass',
		'lambda',
	]
	
	return_keywords := [
		'return',
		'yield',
	]
	
	import_keywords := [
		'import',
		'himport',
		'cimport',
		'from',
		'as',
	]
	
	exception_keywords := [
		'try',
		'except',
		'raise',
		'finally',
		'assert',
	]
	
	asynchronous_keywords := [
		'async',
		'await',
	]

	variable_handling_keywords := [
		'del',
		'global',
		'nonlocal',
	]
	
	datatypes := [
		// Binary Types
		'memoryview', 
		'bytearray', 
		'bytes'.
		
		// Numeric Types
		'complex', 
		'float', 
		'int',
		
		// Text Type
		'str',
		
		// Boolean Type
		'bool',
		
		// Set Types
		'frozenset',
		'set',
		
		// Mapping Type
		'dict',
		
		// Sequence Types
		'range', 
		'tuple', 
		'list',
	]

	// defining an empty array
	mut collection_of_keywords := []string{}

	// appending the keywords
	collection_of_keywords << arithmetic_operators
	collection_of_keywords << assignment_operators
	collection_of_keywords << comparison_operators
	collection_of_keywords << logical_operators
	collection_of_keywords << identity_operators
	collection_of_keywords << membership_operators
	collection_of_keywords << bitwise_operators
	collection_of_keywords << value_keywords
	collection_of_keywords << conditional_keywords
	collection_of_keywords << iteration_keywords
	collection_of_keywords << structure_keywords
	collection_of_keywords << return_keyword
	collection_of_keywords << import_keywords
	collection_of_keywords << exception_keywords
	collection_of_keywords << asynchronous_keywords
	collection_of_keywords << variable_handling_keywords
	collection_of_keywords << datatypes
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
