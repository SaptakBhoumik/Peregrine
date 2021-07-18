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
			','
			] 
logical_operators=[
					'=',//equals
					'+',//addition
					'-',//subtraction
					'*',//multiplication
					'/'//division
					'^',//exponent
					'//'//quotient
					'%'//remainder
					]
other_symbols := ['\\', '/*', '*/'] 
keywords := ['public', 'class', 'void', 'main', 'String', 'int']
mut collection_of_keywords := []string{}
collection_of_keywords << symbols
collection_of_keywords << other_symbols 
collection_of_keywords << keywords 
println(collection_of_keywords)