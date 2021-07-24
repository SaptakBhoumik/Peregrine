import tokenizer
a:="
for i,char in enumerate(string):
    if char != white_space:
		# adding a char each time
        lexeme += char 
    #print(lexeme)
	# prevents error
    if i+1 < len(string):
        if string[i+1] == white_space or string[i+1] in KEYWORDS or lexeme in KEYWORDS: 
                    #sds
            if lexeme != '':
                a.append(lexeme.replace('\n', '<newline>'))
                print(lexeme.replace('\n', '<newline>'))
                lexeme = ''
print(a)
"
k:=tokenizer.tokenize(a)
println(k)
if "#" in k{
    print("fail")
}
else {
    print("pass")
}