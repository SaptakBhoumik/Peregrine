module tokenizer

pub fn remove_white_space(list []string) []string{
	mut lis := list.clone()
	//defining an empty array
	mut results := [" "]
	for i,mut item in lis{
		mut count:=results.len
		if i>0{
			mut prev_item:= results[count-1]
			if count>0{
				if prev_item !=" " && item!=" "{
					unsafe{
					 results[count-1]+=*item
					}
				}
				else if prev_item !=" " && item==" "{
					results << item
				}
				else if prev_item==" "{
					results << item
				}
			}
			else if count==0{
				results << item
			}
		}
	}
	return results
}