module tokenizer
// Original author: Saptak Bhoumik
fn symbol_present_in_arrey(item string) int{
	//it finds if the item is any one of the symbol
	symbols:=[
					'!',
					'Ccode',
					'#',
					'=',//equals
					'+',//addition
					'-',//subtraction
					'*',//multiplication
					'/',//division
					'^',//exponent
					'(',
					'.',
					',',
					')',
					':',
					'%',//remainder
					'>',//greater than
					'<',//less than
					'//',//quotient
					'==',
					'>=',//greater than equals to
					'<=',//less than equals to
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
//it finds if the item is a number
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
//it organizes the partially tokenized text
	mut is_double_quote_open:=false//checks if double quote " is open
	mut is_single_quote_open:=false//checks if single quote ' is open
	mut is_ccode:=false
	mut is_double_quote_open_list:=false
	mut is_single_quote_open_list:=false
	mut is_double_quote_open_dictionary:=false
	mut is_single_quote_open_dictionary:=false
	mut is_list_open:=false//checks if it is a list
	mut is_dictionary_open:=false//checks if is is a dictionary
	multi_char_symbols:=[
					'!='//not equals to
					'//',//quotient
					'==',
					'>=',//greater than equals to
					'<=',//less than equals to
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
				if consequtive_item in multi_char_symbols && is_double_quote_open==false && is_single_quote_open==false && is_list_open==false && is_dictionary_open==false && is_double_quote_open_list==false && is_single_quote_open_list==false && is_double_quote_open_dictionary==false && is_single_quote_open_dictionary==false && is_ccode==false{
					unsafe{
					 results[count-1]+=*item
					}
				}
				else if prev_item !=" " && item!=" " && symbol_present_in_arrey(item)==0 && symbol_present_in_arrey(prev_item)==0 && is_double_quote_open==false && is_single_quote_open==false && is_list_open==false && is_dictionary_open==false && item!=r"\n" && prev_item !=r"\n" && is_double_quote_open_list==false && is_single_quote_open_list==false && is_double_quote_open_dictionary==false && is_single_quote_open_dictionary==false && is_ccode==false{
					unsafe{
					 results[count-1]+=*item
					}
				}
				else if item=="." && number_present_in_arrey(prev_item)==1 && is_double_quote_open==false && is_single_quote_open==false && is_list_open==false && is_dictionary_open==false && is_double_quote_open_list==false && is_single_quote_open_list==false && is_double_quote_open_dictionary==false && is_single_quote_open_dictionary==false && is_ccode==false{
					unsafe{
					 results[count-1]+=*item
					}				
				}
				else if prev_item=="." && number_present_in_arrey(item)==1 && is_double_quote_open==false && is_single_quote_open==false && is_list_open==false && is_dictionary_open==false && is_double_quote_open_list==false && is_single_quote_open_list==false && is_double_quote_open_dictionary==false && is_single_quote_open_dictionary==false && is_ccode==false{
					unsafe{
					 results[count-1]+=*item
					}				
				}
				//single quote
				else if item=="'" && is_double_quote_open==false && is_single_quote_open==false  && is_list_open==false && is_dictionary_open==false && is_double_quote_open_list==false && is_single_quote_open_list==false && is_double_quote_open_dictionary==false && is_single_quote_open_dictionary==false && is_ccode==false{
					is_single_quote_open=true
					results << item
				}
				else if item!="'" && is_double_quote_open==false && is_single_quote_open==true  && is_list_open==false && is_dictionary_open==false && is_double_quote_open_list==false && is_single_quote_open_list==false && is_double_quote_open_dictionary==false && is_single_quote_open_dictionary==false && is_ccode==false{
					unsafe{
					 results[count-1]+=*item
					}
				}
				else if item=="'" && is_double_quote_open==false && is_single_quote_open==true  && is_list_open==false && is_dictionary_open==false && is_double_quote_open_list==false && is_single_quote_open_list==false && is_double_quote_open_dictionary==false && is_single_quote_open_dictionary==false && is_ccode==false{
					is_single_quote_open=false
					unsafe{
					 results[count-1]+=*item
					}
				}
				//double quote
				else if item=='"' && is_double_quote_open==false && is_single_quote_open==false  && is_list_open==false && is_dictionary_open==false && is_double_quote_open_list==false && is_single_quote_open_list==false && is_double_quote_open_dictionary==false && is_single_quote_open_dictionary==false && is_ccode==false{
					is_double_quote_open=true
					results << item
				}
				else if item!='"' && is_double_quote_open==true && is_single_quote_open==false  && is_list_open==false && is_dictionary_open==false && is_double_quote_open_list==false && is_single_quote_open_list==false && is_double_quote_open_dictionary==false && is_single_quote_open_dictionary==false && is_ccode==false{
					unsafe{
					 results[count-1]+=*item
					}
				}
				else if item=='"' && is_double_quote_open==true && is_single_quote_open==false   && is_list_open==false && is_dictionary_open==false && is_double_quote_open_list==false && is_single_quote_open_list==false && is_double_quote_open_dictionary==false && is_single_quote_open_dictionary==false && is_ccode==false{
					is_double_quote_open=false
					unsafe{
					 results[count-1]+=*item
					}
				}
				//c code
				else if item=="Ccode" && is_double_quote_open==false && is_single_quote_open==false  && is_list_open==false && is_dictionary_open==false && is_double_quote_open_list==false && is_single_quote_open_list==false && is_double_quote_open_dictionary==false && is_single_quote_open_dictionary==false && is_ccode==false{
					is_ccode=true
					mut enter:="\n"
					results << item
					results << enter
				}
				else if item!="Ccode" && is_double_quote_open==false && is_single_quote_open==false  && is_list_open==false && is_dictionary_open==false && is_double_quote_open_list==false && is_single_quote_open_list==false && is_double_quote_open_dictionary==false && is_single_quote_open_dictionary==false && is_ccode==true{
					if item==r"\n"{
						item="\n"
					}
					unsafe{
					 results[count-1]+=*item
					}
				}
				else if item=="Ccode" && is_double_quote_open==false && is_single_quote_open==false  && is_list_open==false && is_dictionary_open==false && is_double_quote_open_list==false && is_single_quote_open_list==false && is_double_quote_open_dictionary==false && is_single_quote_open_dictionary==false && is_ccode==true{
					is_ccode=false
					results << item
				}
				//list
				else if item=='[' && is_double_quote_open==false && is_single_quote_open==false  && is_list_open==false && is_dictionary_open==false && is_double_quote_open_list==false && is_single_quote_open_list==false && is_double_quote_open_dictionary==false && is_single_quote_open_dictionary==false && is_ccode==false{
					is_list_open=true
					results << item
				}
				else if item!=']' && is_double_quote_open==false && is_single_quote_open==false  && is_list_open==true && is_dictionary_open==false  && is_double_quote_open_list==false && is_single_quote_open_list==false && is_double_quote_open_dictionary==false && is_single_quote_open_dictionary==false && is_ccode==false{
					unsafe{
					 results[count-1]+=*item
					}
				}
				else if item=='"' && is_double_quote_open==false && is_single_quote_open==false  && is_list_open==true && is_dictionary_open==false  && is_double_quote_open_list==false && is_single_quote_open_list==false && is_double_quote_open_dictionary==false && is_single_quote_open_dictionary==false && is_ccode==false{
					is_double_quote_open_list=true
					unsafe{
					 results[count-1]+=*item
					}
				}
				else if item!='"' && is_double_quote_open==false && is_single_quote_open==false  && is_list_open==true && is_dictionary_open==false  && is_double_quote_open_list==true && is_single_quote_open_list==false && is_double_quote_open_dictionary==false && is_single_quote_open_dictionary==false && is_ccode==false{
					unsafe{
					 results[count-1]+=*item
					}
				}
				else if item=='"' && is_double_quote_open==false && is_single_quote_open==false  && is_list_open==true && is_dictionary_open==false  && is_double_quote_open_list==true && is_single_quote_open_list==false && is_double_quote_open_dictionary==false && is_single_quote_open_dictionary==false && is_ccode==false{
					is_double_quote_open_list=false
					unsafe{
					 results[count-1]+=*item
					}
				}
				else if item=="'" && is_double_quote_open==false && is_single_quote_open==false  && is_list_open==true && is_dictionary_open==false  && is_double_quote_open_list==false && is_single_quote_open_list==false && is_double_quote_open_dictionary==false && is_single_quote_open_dictionary==false && is_ccode==false{
					is_single_quote_open_list=true
					unsafe{
					 results[count-1]+=*item
					}
				}
				else if item!="'" && is_double_quote_open==false && is_single_quote_open==false  && is_list_open==true && is_dictionary_open==false  && is_double_quote_open_list==false && is_single_quote_open_list==true && is_double_quote_open_dictionary==false && is_single_quote_open_dictionary==false && is_ccode==false{
					unsafe{
					 results[count-1]+=*item
					}
				} 
				else if item=="'" && is_double_quote_open==false && is_single_quote_open==false  && is_list_open==true && is_dictionary_open==false  && is_double_quote_open_list==false && is_single_quote_open_list==true && is_double_quote_open_dictionary==false && is_single_quote_open_dictionary==false && is_ccode==false{
					is_single_quote_open_list=false
					unsafe{
					 results[count-1]+=*item
					}
				}
				else if item==']' && is_double_quote_open==false && is_single_quote_open==false   && is_list_open==true && is_dictionary_open==false  && is_double_quote_open_list==false && is_single_quote_open_list==false && is_double_quote_open_dictionary==false && is_single_quote_open_dictionary==false && is_ccode==false{
					is_list_open=false
					unsafe{
					 results[count-1]+=*item
					}
				} 
				//dictionary
				else if item=='{' && is_double_quote_open==false && is_single_quote_open==false  && is_list_open==false && is_dictionary_open==false && is_double_quote_open_list==false && is_single_quote_open_list==false && is_double_quote_open_dictionary==false && is_single_quote_open_dictionary==false && is_ccode==false{
					is_dictionary_open=true
					results << item
				}
				else if item!='}' && is_double_quote_open==false && is_single_quote_open==false  && is_list_open==false && is_dictionary_open==true  && is_double_quote_open_list==false && is_single_quote_open_list==false && is_double_quote_open_dictionary==false && is_single_quote_open_dictionary==false && is_ccode==false{
					unsafe{
					 results[count-1]+=*item
					}
				}
				else if item=='"' && is_double_quote_open==false && is_single_quote_open==false  && is_list_open==false && is_dictionary_open==true  && is_double_quote_open_list==false && is_single_quote_open_list==false && is_double_quote_open_dictionary==false && is_single_quote_open_dictionary==false && is_ccode==false{
					is_double_quote_open_dictionary=true
					unsafe{
					 results[count-1]+=*item
					}
				}
				else if item!='"' && is_double_quote_open==false && is_single_quote_open==false  && is_list_open==false && is_dictionary_open==true  && is_double_quote_open_list==false && is_single_quote_open_list==false && is_double_quote_open_dictionary==true && is_single_quote_open_dictionary==false && is_ccode==false{
					unsafe{
					 results[count-1]+=*item
					}
				}
				else if item=='"' && is_double_quote_open==false && is_single_quote_open==false  && is_list_open==false && is_dictionary_open==true  && is_double_quote_open_list==false && is_single_quote_open_list==false && is_double_quote_open_dictionary==true && is_single_quote_open_dictionary==false && is_ccode==false{
					is_double_quote_open_dictionary=false
					unsafe{
					 results[count-1]+=*item
					}
				}
				else if item=="'" && is_double_quote_open==false && is_single_quote_open==false  && is_list_open==false && is_dictionary_open==true  && is_double_quote_open_list==false && is_single_quote_open_list==false && is_double_quote_open_dictionary==false && is_single_quote_open_dictionary==false && is_ccode==false{
					is_single_quote_open_dictionary=true
					unsafe{
					 results[count-1]+=*item
					}
				}
				else if item!="'" && is_double_quote_open==false && is_single_quote_open==false  && is_list_open==false && is_dictionary_open==true  && is_double_quote_open_list==false && is_single_quote_open_list==false && is_double_quote_open_dictionary==false && is_single_quote_open_dictionary==true && is_ccode==false{
					unsafe{
					 results[count-1]+=*item
					}
				} 
				else if item=="'" && is_double_quote_open==false && is_single_quote_open==false  && is_list_open==false && is_dictionary_open==true  && is_double_quote_open_list==false && is_single_quote_open_list==false && is_double_quote_open_dictionary==false && is_single_quote_open_dictionary==true && is_ccode==false{
					is_single_quote_open_dictionary=false
					unsafe{
					 results[count-1]+=*item
					}
				}
				else if item=='}' && is_double_quote_open==false && is_single_quote_open==false   && is_list_open==false && is_dictionary_open==true  && is_double_quote_open_list==false && is_single_quote_open_list==false && is_double_quote_open_dictionary==false && is_single_quote_open_dictionary==false && is_ccode==false{
					is_dictionary_open=false
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
	mut is_comment_open:=false
	mut final_result:=[]string{}
	//removing comment
	for mut item in results{
		if item=="#" && is_comment_open==false{
			is_comment_open=true
		}
		else if item!=r"\n" && is_comment_open==true{
			continue
		}
		else if item==r"\n" && is_comment_open==true{
			is_comment_open=false
			final_result << item
		}
		else{
			final_result << item
		}
	}
	return final_result 
}