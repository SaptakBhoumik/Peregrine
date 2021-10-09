module lexer

pub enum Token_type {
			space=0
			newline=1
			bracket=2
			peregrine_type=3
			comma=4
			dot=5
			decorator=6
			operator=7
			colon=8
			loop=9
			logic=10
			def=11
			struct_name=12
			class=13
			constant=14
			ccode=15
			pass=16
			str=17
			integer=18
			decimal=19
			pointer=20
			null=21
			list=22
			dictionary=23
			true_false=24
			unknown=25
			}
pub struct Lex{
pub mut: 
	keyword string
	keyword_type Token_type
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
		match item{
		" "{
			lex_item.keyword_type = Token_type.space
			}
		r"\n"{
			lex_item.keyword_type=Token_type.newline
		}
		")"{
			lex_item.keyword_type = Token_type.bracket
		}
		 "("{
			lex_item.keyword_type = Token_type.bracket
		}
		","{
			lex_item.keyword_type =Token_type.comma
		}
		"."{
			lex_item.keyword_type =Token_type.dot
		}
		":"{
			lex_item.keyword_type = Token_type.colon
		}
		"def"{
			lex_item.keyword_type = Token_type.def
		}
		"struct"{
			lex_item.keyword_type = Token_type.struct_name 
		}
		"class"{
			lex_item.keyword_type = Token_type.class
		}"const"{
			lex_item.keyword_type= Token_type.constant
		}"Ccode"{
			lex_item.keyword_type = Token_type.ccode 
		}"pass"{
			lex_item.keyword_type = Token_type.pass 
		}
		else{
			if item in peregrine_type{
				lex_item.keyword_type=Token_type.peregrine_type
			}
			else if item.split("")[0] =="@"{
				lex_item.keyword_type=Token_type.decorator
			}
			else if item in operater{
				lex_item.keyword_type = Token_type.operator
			}
			else if item in loop{
				lex_item.keyword_type = Token_type.loop
			}
			 else if item in logic{
				lex_item.keyword_type = Token_type.logic
			}else{
				lex_item.keyword_type = know_type(item)
			}
		}
	}
		lex_item.keyword=item
		collection<<lex_item
		lex_item=Lex{}
	
	}
	return collection
}