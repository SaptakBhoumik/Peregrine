from dataclasses import dataclass

# peregrine has builtin support for structure so it is not needed when we write in peregrine
from tokens import *


@dataclass
class Token:  # change class to struct
    keyword: str = ""
    index: int = 0
    line: int = 1
    tk_type: int = 0
    tab: float = 0


def token_creater(
    keyword: str = "", index: int = 0, line: int = 1, tab: float = 0, tk_type: int = 0
):
    x = Token()
    x.keyword = keyword
    x.index = index
    x.line = line
    x.tk_type = tk_type
    x.tab = tab
    return x

def equal(
    keyword: str = "", index: int = 0, line: int = 1, tab: float = 0, 
):
    x = Token()
    x.index = index
    x.line = line
    x.tab = tab

    if keyword=="!":
        x.keyword = "!="
        x.tk_type = TokenType.tk_not_equal
    elif keyword=="/":
        x.keyword = "/="
        x.tk_type = TokenType.tk_slash_equal
    elif keyword=="//":
        x.keyword = "//="
        x.tk_type = TokenType.tk_floor_equal
    elif keyword=="+":
        x.keyword = "+="
        x.tk_type = TokenType.tk_plus_equal
    elif keyword=="-":
        x.keyword = "-="
        x.tk_type = TokenType.tk_minus_equal
    elif keyword=="*":
        x.keyword = "*="
        x.tk_type = TokenType.tk_times_equal
    elif keyword=="%":
        x.keyword = "%="
        x.tk_type = TokenType.tk_mod_equal
    elif keyword=="<<":
        x.keyword = "<<="
        x.tk_type = TokenType.tk_shift_left_equal
    elif keyword==">>":
        x.keyword = ">>="
        x.tk_type = TokenType.tk_shift_right_equal
    elif keyword=="&":
        x.keyword = "&="
        x.tk_type = TokenType.tk_bit_and_equal
    elif keyword=="|":
        x.keyword = "|="
        x.tk_type = TokenType.tk_bit_or_equal
    elif keyword=="^":
        x.keyword = "^="
        x.tk_type = TokenType.tk_bit_xor_equal
    return x

def lexer(code: str) -> list:
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
    token = token_creater()
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
                    token = token_creater(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=TokenType.array
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
                    token = token_creater(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=TokenType.dictionary
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
                token = token_creater(
                    keyword=keyword, index=index, line=line, tab=tab, tk_type=TokenType.string
                )
                string_starter=""
            else:
                is_string = True
                index=current_index
                keyword = item
                index = current_index
                string_starter = item

        elif item=="(":
            token = token_creater(
                    keyword=keyword, index=index, line=line, tab=tab, tk_type=TokenType.tk_l_paren
                )
        
        elif item==")":
            token = token_creater(
                    keyword=keyword, index=index, line=line, tab=tab, tk_type=TokenType.tk_l_paren
                )
        elif item=="+":
            if keyword=="+":
                token = token_creater(
                        keyword="++", index=index, line=line, tab=tab, tk_type=TokenType.tk_increment
                    )
            elif next(current_index,code)=="+" or  next(current_index,code)=="=" :
                keyword=item
                index = current_index
            else:
                index = current_index
                keyword=item
                token = token_creater(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=TokenType.tk_plus
                    )
        elif item==">":
            if keyword==">":
                if next(current_index,code)=="=":
                    keyword=">>"
                else:
                    token = token_creater(
                            keyword=">>", index=index, line=line, tab=tab, tk_type=TokenType.tk_shift_right
                        )
            elif keyword=="-":
                token = token_creater(
                            keyword="->", index=index, line=line, tab=tab, tk_type=TokenType.tk_arrow
                        )
            elif next(current_index,code)==">" or  next(current_index,code)=="=" :
                keyword=item
                index = current_index
            else:
                index = current_index
                keyword=item
                token = token_creater(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=TokenType.tk_greater
                    )
        elif item=="<":
            if keyword=="<":
                if next(current_index,code)=="=":
                    keyword="<<"
                else:
                    token = token_creater(
                            keyword="<<", index=index, line=line, tab=tab, tk_type=TokenType.tk_shift_left
                        )
            elif next(current_index,code)=="<" or  next(current_index,code)=="=" :
                keyword=item
                index = current_index
            else:
                index = current_index
                keyword=item
                token = token_creater(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=TokenType.tk_less
                    )
        elif item=="-":
            if keyword=="-":
                token = token_creater(
                        keyword="--", index=index, line=line, tab=tab, tk_type=TokenType.tk_decrement
                    )
            elif next(current_index,code)=="-" or next(current_index,code)==">" or   next(current_index,code)=="=" :
                keyword=item
                index = current_index
            else:
                index = current_index
                keyword=item
                token = token_creater(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=TokenType.tk_minus
                    )
        elif item=="=":
            if keyword=="=":
                keyword="=="
                token = token_creater(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=TokenType.tk_equal
                    )
            elif next(current_index,code)=="=":
                keyword=item
                index = current_index
            elif keyword=="":
                keyword=item
                token = token_creater(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=TokenType.tk_assign
                    )
            else:
                token = equal(
                        keyword=keyword, index=index, line=line, tab=tab
                    )

        elif item=="/":
            if keyword=="/":
                if next(current_index,code)=="=":
                    keyword="//"
                else:
                    token = token_creater(
                            keyword="//", index=index, line=line, tab=tab, tk_type=TokenType.tk_floor
                        )
            elif next(current_index,code)=="/" or  next(current_index,code)=="=" :
                keyword=item
                index = current_index
            else:
                index = current_index
                keyword=item
                token = token_creater(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=TokenType.tk_divide
                    )
        elif item=="^":
            if next(current_index,code)=="=":
                keyword=item
                index = current_index
            else:
                index = current_index
                keyword=item
                token = token_creater(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=TokenType.tk_xor
                    )
        elif item=="%":
            if next(current_index,code)=="=":
                keyword=item
                index = current_index
            else:
                index = current_index
                keyword=item
                token = token_creater(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=TokenType.tk_modulo
                    )
        elif item=="&":
            if next(current_index,code)=="=":
                keyword=item
                index = current_index
            else:
                index = current_index
                keyword=item
                token = token_creater(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=TokenType.tk_ampersand
                    )
        elif item=="|":
            if next(current_index,code)=="=":
                keyword=item
                index = current_index
            else:
                index = current_index
                keyword=item
                token = token_creater(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=TokenType.tk_bit_or
                    )
        elif item=="~":
            index = current_index
            keyword=item
            token = token_creater(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=TokenType.tk_bit_not
                    )
        elif item=="!":
            index = current_index
            keyword=item

        elif item==":":
            index = current_index
            keyword=item
            token = token_creater(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=TokenType.tk_colon
                    )
            
        elif item==",":
            index = current_index
            keyword=item
            token = token_creater(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=TokenType.tk_comma
                    )

        else:
            pass


        if token.keyword != "":
            tokens.append(token)
            token = token_creater()
            keyword = ""
    return tokens


print(lexer("code++ + -- -// / & ^ |~ :,//= >>="))