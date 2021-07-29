module tokenizer

fn symbol_present_in_arrey(item string) int{
	symbols:=[
					'=',//equals
					'+',//addition
					'-',//subtraction
					'*',//multiplication
					'/',//division
					'^',//exponent
					'(',
					'.',
					')',
					':',
					'!',
					'%',//remainder
					'>',//greater than
					'<',//less than
					'//',//quotient
					'==',
					'=>',//greater than equals to
					'=<',//less than equals to
					'!='//not equals to
					]
	mut present:=0//false
	for symbol in symbols{
		if item==symbol{
			present=1//true
			break
		}
		else{
			continue
		}
	}
	return present
}

fn number_present_in_arrey(item string) int{
	symbols:=[
				'0',
				'1',
				'2',
				'3',
				'4',
				'5',
				'6',
				'7',
				'8',
				'9'
				]
	mut present:=0//false
	for symbol in symbols{
		if item==symbol{
			present=1//true
			break
		}
		else{
			continue
		}
	}
	return present
}

pub fn process_tokens(list []string) []string{

	mut is_double_quote_open:=false//checks if double quote " is open
	mut is_single_quote_open:=false//checks if single quote ' is open
	mut is_list_open:=false//checks if it is a list
	mut is_dictionary_open:=false//checks if is is a dictionary
	multi_char_symbols:=[
					'//',//quotient
					'==',
					'=>',//greater than equals to
					'=<',//less than equals to
					'!='//not equals to
					]
	mut lis := list.clone()
	//defining an empty array
	mut results := []string{}
	for i,mut item in lis{
		mut count:=results.len
		
		if i>0{		
			if count>0{
				mut prev_item:= results[count-1]
				mut consequtive_item:="$prev_item$item"
				//i know this part is a bit messy
				if consequtive_item in multi_char_symbols && is_double_quote_open==false && is_single_quote_open==false && is_list_open==false && is_dictionary_open==false{
					unsafe{
					 results[count-1]+=*item
					}
				}
				else if prev_item !=" " && item!=" " && symbol_present_in_arrey(item)==0 && symbol_present_in_arrey(prev_item)==0 && is_double_quote_open==false && is_single_quote_open==false && is_list_open==false && is_dictionary_open==false && item!=r"\n" && prev_item !=r"\n"{
					unsafe{
					 results[count-1]+=*item
					}				
				}
				else if item=="." && number_present_in_arrey(prev_item)==1 && is_double_quote_open==false && is_single_quote_open==false && is_list_open==false && is_dictionary_open==false{
					unsafe{
					 results[count-1]+=*item
					}				
				}
				else if prev_item=="." && number_present_in_arrey(item)==1 && is_double_quote_open==false && is_single_quote_open==false && is_list_open==false && is_dictionary_open==false{
					unsafe{
					 results[count-1]+=*item
					}				
				}
				else{
					results << item
				}
			}
		}
		else{
			results << item
			}
	}
	return results
}