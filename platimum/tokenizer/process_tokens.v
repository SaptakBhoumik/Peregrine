module tokenizer

pub fn process_tokens(list []string) []string{
	mut lis := list.clone()
	//defining an empty array
	mut results := []string{}
	for i,mut item in lis{
		mut count:=results.len
		if i>0{		
			if count>0{
				mut prev_item:= results[count-1]
				if prev_item !=" " && item!=" "{
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