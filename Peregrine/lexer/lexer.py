from dataclasses import dataclass

# peregrine has builtin support for structure so it is not needed when we write in peregrine
from .tokens import *
from errors import errors


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

def is_number(item:str)->str:
    number:list = ["1","2","3","4","5","6","7","8","9"]
    if item[0] in number:
        if len(item.split("."))==2:
            return TokenType.decimal
        else:
            return TokenType.integer
    else:
        return TokenType.tk_identifier

def token_type(item : str) -> int:
    if item=="True":
        return TokenType.tk_true
    elif item=="False":
        return TokenType.tk_false
    elif item=="None":
        return TokenType.tk_none
    elif item=="import":
        return TokenType.tk_import
    elif item=="const":
        return TokenType.tk_const
    elif item=="if":
        return TokenType.tk_if
    elif item=="elif":
        return TokenType.tk_elif
    elif item=="else":
        return TokenType.tk_else
    elif item=="while":
        return TokenType.tk_while
    elif item=="for":
        return TokenType.tk_for
    elif item=="break":
        return TokenType.tk_break
    elif item=="continue":
        return TokenType.tk_continue
    elif item=="match":
        return TokenType.tk_match
    elif item=="case":
        return TokenType.tk_case
    elif item=="default":
        return TokenType.tk_default
    elif item=="def":
        return TokenType.tk_def
    elif item=="pass":
        return TokenType.tk_pass
    elif item=="return":
        return TokenType.tk_return
    elif item=="and":
        return TokenType.tk_and
    elif item=="or":
        return TokenType.tk_or
    elif item=="not":
        return TokenType.tk_not
    elif item=="is":
        return TokenType.tk_is
    elif item=="in":
        return TokenType.tk_in
    elif item=="Ccode":
        return TokenType.tk_ccode
    elif item=="class":
        return TokenType.tk_class
    elif item=="struct":
        return TokenType.tk_struct
    elif item=="str":
        return TokenType.tk_str
    elif item=="bool":
        return TokenType.tk_bool
    elif item=="char":
        return TokenType.tk_char
    elif item=="float":
        return TokenType.tk_float
    elif item=="float32":
        return TokenType.tk_float32
    elif item=="void":
        return TokenType.tk_void
    elif item=="int":
        return TokenType.tk_int
    elif item=="int32":
        return TokenType.tk_int32
    elif item=="int16":
        return TokenType.tk_int16
    elif item=="int8":
        return TokenType.tk_int8
    elif item=="uint":
        return TokenType.tk_uint
    elif item=="uint32":
        return TokenType.tk_uint32
    elif item=="uint16":
        return TokenType.tk_uint16
    elif item=="uint8":
        return TokenType.tk_uint8
    else:  
        return is_number(item)

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
    is_comment:bool=False
    operator:list=["!","/","//","+","-","*","%","<<",">>","&","|","^"]
    for current_index, item in enumerate(code):
        if item == " " and is_tab == True:
            tab += 0.25
        elif item != " " and is_tab == True:
            is_tab = False
        elif item == "\n" or item == "\r\n":
            tab = 0
            line += 1
            is_tab = True

        if item=="#" and is_string==False and is_list_dictionary_string==False and is_comment==False:
            is_comment=True
        elif is_comment==True:
            if item=="\n" or item=="\r\n":
                is_comment=False

        elif is_string == True and string_starter != item:
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
            if keyword!="":
                token = token_creater(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=token_type(keyword)
                    )


        elif item == "[" and is_dictionary == False  and is_string == False  and is_array == False:
            if keyword!="":
                token = token_creater(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=token_type(keyword)
                    )
                tokens.append(token)
                token = token_creater()
                keyword = ""
            index=current_index
            keyword = item
            is_array = True
            second_bracket_count += 1
        

        elif item == "{" and is_dictionary == False  and is_string == False  and is_array == False:
            if keyword!="":
                token = token_creater(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=token_type(keyword)
                    )
                tokens.append(token)
                token = token_creater()
                keyword = ""
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
                if keyword!="":
                    token = token_creater(
                            keyword=keyword, index=index, line=line, tab=tab, tk_type=token_type(keyword)
                        )
                    tokens.append(token)
                    token = token_creater()
                    keyword = ""
                is_string = True
                index=current_index
                keyword = item
                index = current_index
                string_starter = item

        elif item=="(":
            if keyword!="":
                token = token_creater(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=token_type(keyword)
                    )
                tokens.append(token)
                token = token_creater()
                keyword = ""
            token = token_creater(
                    keyword=keyword, index=index, line=line, tab=tab, tk_type=TokenType.tk_l_paren
                )
        elif item==".":
            if keyword=="":
                keyword=item
                index=current_index
                token = token_creater(
                    keyword=keyword, index=index, line=line, tab=tab, tk_type=TokenType.tk_l_paren
                ) 
            else:
                if is_number(keyword)==TokenType.integer:
                    keyword+=item
                else:
                    token = token_creater(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=token_type(keyword)
                    )
                    tokens.append(token)
                    token = token_creater()
                    keyword = ""
                    keyword=item
                    index=current_index
                    token = token_creater(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=TokenType.tk_l_paren
                    )

        elif item==")":
            if keyword!="":
                token = token_creater(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=token_type(keyword)
                    )
                tokens.append(token)
                token = token_creater()
                keyword = ""
            token = token_creater(
                    keyword=keyword, index=index, line=line, tab=tab, tk_type=TokenType.tk_l_paren
                )
        elif item=="+":
            if keyword!="" and keyword!="+":
                token = token_creater(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=token_type(keyword)
                    )
                tokens.append(token)
                token = token_creater()
                keyword = ""
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
            if keyword!="" and keyword!=">":
                token = token_creater(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=token_type(keyword)
                    )
                tokens.append(token)
                token = token_creater()
                keyword = ""
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
            if keyword!="" and keyword!="<":
                token = token_creater(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=token_type(keyword)
                    )
                tokens.append(token)
                token = token_creater()
                keyword = ""
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
            if keyword!="" and keyword!="-":
                token = token_creater(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=token_type(keyword)
                    )
                tokens.append(token)
                token = token_creater()
                keyword = ""
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
            if keyword not in operator and keyword!="":
                token = token_creater(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=token_type(keyword)
                    )
                tokens.append(token)
                token = token_creater()
                keyword = ""
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
            if keyword!="" and keyword!="/":
                token = token_creater(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=token_type(keyword)
                    )
                tokens.append(token)
                token = token_creater()
                keyword = ""
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
            if keyword!="":
                token = token_creater(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=token_type(keyword)
                    )
                tokens.append(token)
                token = token_creater()
                keyword = ""
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
            if keyword!="":
                token = token_creater(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=token_type(keyword)
                    )
                tokens.append(token)
                token = token_creater()
                keyword = ""
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
            if keyword!="":
                token = token_creater(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=token_type(keyword)
                    )
                tokens.append(token)
                token = token_creater()
                keyword = ""
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
            if keyword!="":
                token = token_creater(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=token_type(keyword)
                    )
                tokens.append(token)
                token = token_creater()
                keyword = ""
            if next(current_index,code)=="=":
                keyword=item
                index = current_index
            else:
                index = current_index
                keyword=item
                token = token_creater(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=TokenType.tk_bit_or
                    )
        elif item=="*":
            if keyword!="" and keyword!="*":
                token = token_creater(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=token_type(keyword)
                    )
                tokens.append(token)
                token = token_creater()
                keyword = ""
            
            if (next(current_index,code)=="*" or next(current_index,code)=="=") and (tokens[-1].tk_type==TokenType.tk_identifier or tokens[-1].tk_type==TokenType.decimal or tokens[-1].tk_type==TokenType.integer or next(current_index,code)=="="):
                keyword="*"
                index=current_index
            elif keyword=="*":
                token = token_creater(
                        keyword="**", index=index, line=line, tab=tab, tk_type=TokenType.tk_exponent
                )
            else:
                index = current_index
                keyword=item
                token = token_creater(
                    keyword=keyword, index=index, line=line , tab=tab,tk_type=TokenType.tk_asterisk
                )
            

        elif item=="~":
            if keyword!="":
                token = token_creater(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=token_type(keyword)
                    )
                tokens.append(token)
                token = token_creater()
                keyword = ""
            index = current_index
            keyword=item
            token = token_creater(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=TokenType.tk_bit_not
                    )
        elif item=="!":
            if keyword!="":
                token = token_creater(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=token_type(keyword)
                    )
                tokens.append(token)
                token = token_creater()
                keyword = ""
            index = current_index
            keyword=item

        elif item==":":
            if keyword!="":
                token = token_creater(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=token_type(keyword)
                    )
                tokens.append(token)
                token = token_creater()
                keyword = ""
            index = current_index
            keyword=item
            token = token_creater(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=TokenType.tk_colon
                    )
            
        elif item==",":
            if keyword!="":
                token = token_creater(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=token_type(keyword)
                    )
                tokens.append(token)
                token = token_creater()
                keyword = ""
            index = current_index
            keyword=item
            token = token_creater(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=TokenType.tk_comma
                    )

        else:
            if keyword=="":
                index=current_index
            keyword+= item


        if token.keyword != "":
            tokens.append(token)
            token = token_creater()
            keyword = ""
    if keyword!="":
        tokens.append(token_creater(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=token_type(keyword)
                    ))

    if is_string==True or is_list_dictionary_string==True:
        error=errors.PEError(errors.Location(line,current_index,"file_name",tokens[-1].keyword),"Unexpected end of file",f"Expecting a {string_starter}",tokens[-1].keyword, hint=f"Adding a {string_starter}",)
        error.display()
        return None
    elif is_dictionary==True:
        error=errors.PEError(errors.Location(line,current_index,"file_name",tokens[-1].keyword),"Unexpected end of file","Expecting a }",tokens[-1].keyword, hint="Adding a }",)
        error.display()
        return None
    elif is_array==True:
        error=errors.PEError(errors.Location(line,current_index,"file_name",tokens[-1].keyword),"Unexpected end of file","Expecting a ]",tokens[-1].keyword, hint="Adding a ]",)
        error.display()
        return None
    else:
        return tokens