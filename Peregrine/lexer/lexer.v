module lexer
pub struct Lex{
pub mut: 
	keyword string
	keyword_type string
}
pub fn lexer(token []string) []Lex{
	peregrine_type := ['char', 'int', 'bool', 'str', 'list', 'dictionary', 'float', 'void', 'int32',
		'int16', 'int8', 'float32', 'uint32', 'uint16', 'uint8', 'uint']
	operater := ['=', '==', '+', '-', '*', '/', '^', '//', '%', '>', '<', '>=', '<=', '!=', '++',
		'--',  '|', '~', '<<', '>>', '+=', '-=', '*=', '/=', '%=', '<<=', '>>=', '&=', '|=',
		'^=']
	loop := ['if', 'while', 'elif', 'else', 'for', 'match', 'case', 'default']
	logic := ['and', 'or', 'not', 'in', 'is']
	mut lex_item:=Lex{}
	mut collection:=[]Lex{}
	for item in token{
		if item==" "{
			lex_item.keyword_type = "SPACE"
		}
		else if item==r"\n"{
			lex_item.keyword_type="NEWLINE"
		}
		else if item==")" || item=="("{
			lex_item.keyword_type = "BRACKET"
		}
		else if item in peregrine_type{
			lex_item.keyword_type="TYPE"
		}
		else if item==","{
			lex_item.keyword_type ="COMMA"
		}
		else if item=="."{
			lex_item.keyword_type ="DOT"
		}
		else if item.split("")[0] =="@"{
			lex_item.keyword_type="DECORATOR"
		}
		else if item in operater{
			lex_item.keyword_type = "OPERATOR"
		}
		else if item ==":"{
			lex_item.keyword_type = "COLON"
		}
		else if item in loop{
			lex_item.keyword_type = "LOOP"
		}
		else if item in logic{
			lex_item.keyword_type = "LOGIC"
		}
		else if item =="def"{
			lex_item.keyword_type = "DEF" 
		}
		else if item =="struct"{
			lex_item.keyword_type = "STRUCT" 
		}
		else if item =="class"{
			lex_item.keyword_type = "CLASS" 
		}else if item =="const"{
			lex_item.keyword_type = "CONSTANT" 
		}else if item =="Ccode"{
			lex_item.keyword_type = "CCODE" 
		}
		else{
			lex_item.keyword_type = know_type(item)
		}
		lex_item.keyword=item
		collection<<lex_item
		lex_item=Lex{}
	}
	return collection
}