"""
*  
*  Copyright (c) Peregrine-lang, 2021. All rights reserved.
*
"""

# peregrine has builtin support for structure so it is not needed when we write in peregrine
from tokens import *

def token_create(
    keyword: str = "", index: int = 0, line: int = 1, tab: float = 0, tk_type: int = 0
) -> Token:
    x = Token(keyword, tk_type, index, line)
    x.tab = tab

    return x

def tokenizer(code: str) -> list:
    is_string: bool = False
    is_list_dictionary_string: bool = False
    second_bracket_count: int = 0
    third_bracket_count: int = 0
    is_array: bool = False
    is_dictionary: bool = False
    string_starter: str = ""
    is_tab: bool = True
    tab: float = 0
    line = 1
    tokens: list = []
    token = token_create()
    index: int = 0
    keyword = ""
    for current_index, item in enumerate(code):
        if item == " " and is_tab == True:
            tab += 0.25
        elif item != " " and is_tab == True:
            is_tab = False
        elif item == "\n" or item == "\r\n":
            tab = 0
            line += 1
            is_tab = True

        if is_string == True and string_starter != item:
            keyword += item

        elif is_array == True:
            keyword += item
            if item=="[" and is_list_dictionary_string==False:
                second_bracket_count += 1
            elif item=="'" or  item=='"':
                if is_list_dictionary_string==True and string_starter!=item:
                    pass
                elif item==string_starter and is_list_dictionary_string==True:
                    is_list_dictionary_string=False
                    string_starter=""
                else:
                    string_starter=item
                    is_list_dictionary_string=True


            elif item == "]" and is_list_dictionary_string==False:
                second_bracket_count -= 1
                if second_bracket_count == 0:
                    is_array = False
                    token = token_create(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=TokenType.tk_array
                    )

        elif is_dictionary == True:
            keyword += item
            if item=="{" and is_list_dictionary_string==False:
                third_bracket_count += 1
            elif item=="'" or  item=='"':
                if is_list_dictionary_string==True and string_starter!=item:
                    pass
                elif item==string_starter and is_list_dictionary_string==True:
                    is_list_dictionary_string=False
                    string_starter=""
                else:
                    string_starter=item
                    is_list_dictionary_string=True


            elif item == "}" and is_list_dictionary_string==False:
                third_bracket_count -= 1
                if third_bracket_count == 0:
                    is_dictionary = False
                    token = token_create(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=TokenType.tk_dictionary
                    )

        elif ( item == " " and is_dictionary == False  and is_array == False and is_string == False) or item == "\n"  or item == "\r\n" :
            pass

        elif item == "[" and is_dictionary == False  and is_string == False  and is_array == False:
            index=current_index
            keyword = item
            is_array = True
            second_bracket_count += 1
        

        elif item == "{" and is_dictionary == False  and is_string == False  and is_array == False:
            index=current_index
            keyword = item
            is_dictionary = True
            third_bracket_count += 1


        elif item == "'" or item == '"' and is_array == False and is_dictionary == False:
            if is_string == True and string_starter == item:
                is_string = False
                keyword += item
                token = token_create(
                    keyword=keyword, index=index, line=line, tab=tab, tk_type=TokenType.tk_str
                )
                string_starter=""
            else:
                is_string = True
                index=current_index
                keyword = item
                index = current_index
                string_starter = item

        else:
            pass

        if token.keyword != "":
            tokens.append(token)
            token = token_create()
            keyword = ""
    return tokens


print(tokenizer('[s,"g]",j] {string:2,"}"}'))
