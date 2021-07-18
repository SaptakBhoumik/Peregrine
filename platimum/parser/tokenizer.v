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
			'#',//comment
			''
			] 
logical_operators=[
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
				]

other_symbols := ['\\', '/*', '*/'] 
keywords := ['public', 'class', 'void', 'main', 'String', 'int']
mut collection_of_keywords := []string{}
collection_of_keywords << symbols
collection_of_keywords << other_symbols 
collection_of_keywords << keywords 
println(collection_of_keywords)