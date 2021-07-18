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
						'PRINT',
						'INPUT',
						'SYSTEM'//run command prompt commands
						]
//defining an empty array
mut collection_of_keywords := []string{}
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
println(collection_of_keywords)