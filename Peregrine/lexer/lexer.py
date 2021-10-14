# peregrine has builtin support for structure so it is not needed when we write in peregrine
from .tokens import *
from errors import error_list
from typing import List

def equal(
    keyword: str = "", index: int = 0, line: int = 1, tab: float = 0, 
):
    x = Token()
    x.index = index
    x.line = line
    x.tab = tab

    if keyword == "!":
        x.keyword = "!="
        x.tk_type = TokenType.tk_not_equal
        
    elif keyword == "/":
        x.keyword = "/="
        x.tk_type = TokenType.tk_slash_equal
        
    elif keyword == "//":
        x.keyword = "//="
        x.tk_type = TokenType.tk_floor_equal
        
    elif keyword == "+":
        x.keyword = "+="
        x.tk_type = TokenType.tk_plus_equal
        
    elif keyword == "-":
        x.keyword = "-="
        x.tk_type = TokenType.tk_minus_equal
        
    elif keyword == "*":
        x.keyword = "*="
        x.tk_type = TokenType.tk_times_equal
        
    elif keyword == "%":
        x.keyword = "%="
        x.tk_type = TokenType.tk_mod_equal
        
    elif keyword == "<<":
        x.keyword = "<<="
        x.tk_type = TokenType.tk_shift_left_equal
        
    elif keyword == ">>":
        x.keyword = ">>="
        x.tk_type = TokenType.tk_shift_right_equal
        
    elif keyword == "&":
        x.keyword = "&="
        x.tk_type = TokenType.tk_bit_and_equal
        
    elif keyword == "|":
        x.keyword = "|="
        x.tk_type = TokenType.tk_bit_or_equal
        
    elif keyword == "^":
        x.keyword = "^="
        x.tk_type = TokenType.tk_bit_xor_equal
        
    return x

def is_number(item: str) -> str:
    numbers: list = ["1", "2", "3", "4", "5", "6", "7", "8", "9"]
    if item[0] in numbers:
        if len(item.split(".")) == 2:
            return TokenType.tk_decimal
        else:
            return TokenType.integer
    else:
        return TokenType.tk_identifier

def token_type(item: str) -> int:
    if item=="r":
        return TokenType.tk_raw
    if item=="f":
        return TokenType.tk_format
    elif item=="True":
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
    elif item=="extern":
        return TokenType.tk_extern
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
    elif item=="Cppcode":
        return TokenType.tk_cppcode
    elif item=="himport":
        return TokenType.tk_himport
    elif item=="cppimport":
        return TokenType.tk_cppimport
    elif item=="class":
        return TokenType.tk_class
    elif item=="struct":
        return TokenType.tk_struct
    elif item=="str":
        return TokenType.tk_type_str
    elif item=="bool":
        return TokenType.tk_type_bool
    elif item=="char":
        return TokenType.tk_type_char
    elif item=="float":
        return TokenType.tk_type_float
    elif item=="float32":
        return TokenType.tk_type_float32
    elif item=="void":
        return TokenType.tk_type_void
    elif item=="int":
        return TokenType.tk_type_int
    elif item=="int32":
        return TokenType.tk_type_int32
    elif item=="int16":
        return TokenType.tk_type_int16
    elif item=="int8":
        return TokenType.tk_type_int8
    elif item=="uint":
        return TokenType.tk_type_uint
    elif item=="uint32":
        return TokenType.tk_type_uint32
    elif item=="uint16":
        return TokenType.tk_type_uint16
    elif item=="uint8":
        return TokenType.tk_type_uint8
    else:  
        return is_number(item)

def lexer(code: str,file_name:str) -> list:
    is_string: bool = False
    is_list_dictionary_cpp_string: bool = False
    second_bracket_count: int = 0
    third_bracket_count: int = 0
    is_array: bool = False
    is_dictionary: bool = False
    string_starter: str = ""
    is_tab: bool = True
    tab: float = 0
    identation_levels: List[int] = []
    curr_identation_level: int = 0
    line = 1
    tokens: list = []
    token = Token()
    index: int = 0
    last_line: int = 0
    keyword = ""
    is_cpp:bool=False
    cpp_bracket_count:int=0
    is_comment:bool=False
    statement:str=""
    operator:list=["!","/","//","+","-","*","%","<<",">>","&","|","^","="]
    for current_index, item in enumerate(code):
        if item!="\n":
            statement+=item
        else:
            statement=""
        if item == " " and is_tab == True:
            tab += 0.25
            curr_identation_level += 1
        elif item != " " and is_tab == True and item!="\n":
            is_tab = False

            previous_identation = 0

            if identation_levels:
                previous_identation = identation_levels[len(identation_levels)-1]

            if curr_identation_level > previous_identation:
                token = Token("", 0, 0, TokenType.tk_ident, 0)
                tokens.append(token)
                
                identation_levels.append(curr_identation_level)

            while curr_identation_level < previous_identation:
                identation_levels.pop()
                
                token = Token("", 0, 0, TokenType.tk_dedent, 0)
                tokens.append(token)

                if identation_levels:
                    if curr_identation_level >= identation_levels[len(identation_levels)-1]:
                        break

                    previous_identation = identation_levels[len(identation_levels)-1]
                else:
                    previous_identation = 0
                

        elif item == "\n" or item == "\r\n":
            tab = 0
            last_line=current_index
            line += 1
            is_tab = True
            curr_identation_level = 0

        if is_cpp == True:
            keyword += item
            if item=="(" and is_list_dictionary_cpp_string==False:
                cpp_bracket_count += 1
            elif item=="'" or  item=='"':
                if is_list_dictionary_cpp_string==True and string_starter!=item:
                    pass
                elif item==string_starter and is_list_dictionary_cpp_string==True:
                    is_list_dictionary_cpp_string=False
                    string_starter=""
                else:
                    string_starter=item
                    is_list_dictionary_cpp_string=True
            elif item == ")" and is_list_dictionary_cpp_string==False:
                cpp_bracket_count -= 1
                if cpp_bracket_count == 0:
                    is_array = False
                    token = Token(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=TokenType.cpp
                    )

        elif item=="#" and is_string==False and is_list_dictionary_cpp_string==False and is_comment==False:
            is_comment=True
        elif is_comment==True:
            if item=="\n" or item=="\r\n":
                is_comment=False

        elif is_string == True and string_starter != item:
            keyword += item


        elif is_array == True:
            keyword += item
            if item=="[" and is_list_dictionary_cpp_string==False:
                second_bracket_count += 1
            elif item=="'" or  item=='"':
                if is_list_dictionary_cpp_string==True and string_starter!=item:
                    pass
                elif item==string_starter and is_list_dictionary_cpp_string==True:
                    is_list_dictionary_cpp_string=False
                    string_starter=""
                else:
                    string_starter=item
                    is_list_dictionary_cpp_string=True


            elif item == "]" and is_list_dictionary_cpp_string==False:
                second_bracket_count -= 1
                if second_bracket_count == 0:
                    is_array = False
                    token = Token(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=TokenType.array
                    )

        elif is_dictionary == True:
            keyword += item
            if item=="{" and is_list_dictionary_cpp_string==False:
                third_bracket_count += 1
            elif item=="'" or  item=='"':
                if is_list_dictionary_cpp_string==True and string_starter!=item:
                    pass
                elif item==string_starter and is_list_dictionary_cpp_string==True:
                    is_list_dictionary_cpp_string=False
                    string_starter=""
                else:
                    string_starter=item
                    is_list_dictionary_cpp_string=True


            elif item == "}" and is_list_dictionary_cpp_string==False:
                third_bracket_count -= 1
                if third_bracket_count == 0:
                    is_dictionary = False
                    token = Token(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=TokenType.dictionary
                    )

        elif ( item == " " and is_dictionary == False  and is_array == False and is_string == False) or item == "\n"  or item == "\r\n" :
            if keyword!="":
                token = Token(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=token_type(keyword)
                    )


        elif item == "[" and is_dictionary == False  and is_string == False  and is_array == False:
            if keyword!="":
                token = Token(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=token_type(keyword)
                    )
                tokens.append(token)
                token = Token()
                keyword = ""
            index=current_index
            keyword = item
            is_array = True
            second_bracket_count += 1
        

        elif item == "{" and is_dictionary == False  and is_string == False  and is_array == False:
            if keyword!="":
                token = Token(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=token_type(keyword)
                    )
                tokens.append(token)
                token = Token()
                keyword = ""
            index=current_index
            keyword = item
            is_dictionary = True
            third_bracket_count += 1


        elif item == "'" or item == '"' and is_array == False and is_dictionary == False:
            if is_string == True and string_starter == item:
                is_string = False
                keyword += item
                token = Token(
                    keyword=keyword, index=index, line=line, tab=tab, tk_type=TokenType.tk_str
                )
                string_starter=""
            else:
                if keyword!="":
                    token = Token(
                            keyword=keyword, index=index, line=line, tab=tab, tk_type=token_type(keyword)
                        )
                    tokens.append(token)
                    token = Token()
                    keyword = ""
                is_string = True
                index=current_index
                keyword = item
                index = current_index
                string_starter = item

        elif item=="(":
            if keyword!="":
                token = Token(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=token_type(keyword)
                    )
                tokens.append(token)
                token = Token()
                keyword = ""
            if tokens[-1].tk_type == TokenType.tk_cppcode:
                is_cpp = True
                cpp_bracket_count=1
                keyword=item
            else:
                keyword=item
                token = Token(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=TokenType.tk_l_paren
                    )
        elif item==".":
            if keyword=="":
                keyword=item
                index=current_index
                token = Token(
                    keyword=keyword, index=index, line=line, tab=tab, tk_type=TokenType.tk_l_paren
                ) 
            else:
                if is_number(keyword)==TokenType.tk_integer:
                    keyword+=item
                else:
                    token = Token(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=token_type(keyword)
                    )
                    tokens.append(token)
                    token = Token()
                    keyword = ""
                    keyword=item
                    index=current_index
                    token = Token(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=TokenType.tk_l_paren
                    )

        elif item==")":
            if keyword!="":
                token = Token(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=token_type(keyword)
                    )
                tokens.append(token)
                token = Token()
                keyword = ""
            keyword=item
            token = Token(
                    keyword=keyword, index=index, line=line, tab=tab, tk_type=TokenType.tk_l_paren
                )
        elif item=="+":
            if keyword!="" and keyword!="+":
                token = Token(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=token_type(keyword)
                    )
                tokens.append(token)
                token = Token()
                keyword = ""
            if keyword=="+":
                token = Token(
                        keyword="++", index=index, line=line, tab=tab, tk_type=TokenType.tk_increment
                    )
            elif next(current_index,code)=="+" or  next(current_index,code)=="=" :
                keyword=item
                index = current_index
            else:
                index = current_index
                keyword=item
                token = Token(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=TokenType.tk_plus
                    )
        elif item==">":
            if keyword!="" and keyword!=">":
                token = Token(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=token_type(keyword)
                    )
                tokens.append(token)
                token = Token()
                keyword = ""
            if keyword==">":
                if next(current_index,code)=="=":
                    keyword=">>"
                else:
                    token = Token(
                            keyword=">>", index=index, line=line, tab=tab, tk_type=TokenType.tk_shift_right
                        )
            elif keyword=="-":
                token = Token(
                            keyword="->", index=index, line=line, tab=tab, tk_type=TokenType.tk_arrow
                        )
            elif next(current_index,code)==">" or  next(current_index,code)=="=" :
                keyword=item
                index = current_index
            else:
                index = current_index
                keyword=item
                token = Token(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=TokenType.tk_greater
                    )
        elif item=="<":
            if keyword!="" and keyword!="<":
                token = Token(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=token_type(keyword)
                    )
                tokens.append(token)
                token = Token()
                keyword = ""
            if keyword=="<":
                if next(current_index,code)=="=":
                    keyword="<<"
                else:
                    token = Token(
                            keyword="<<", index=index, line=line, tab=tab, tk_type=TokenType.tk_shift_left
                        )
            elif next(current_index,code)=="<" or  next(current_index,code)=="=" :
                keyword=item
                index = current_index
            else:
                index = current_index
                keyword=item
                token = Token(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=TokenType.tk_less
                    )
        elif item=="-":
            if keyword!="" and keyword!="-":
                token = Token(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=token_type(keyword)
                    )
                tokens.append(token)
                token = Token()
                keyword = ""
            if keyword=="-":
                token = Token(
                        keyword="--", index=index, line=line, tab=tab, tk_type=TokenType.tk_decrement
                    )
            elif next(current_index,code)=="-" or next(current_index,code)==">" or   next(current_index,code)=="=" :
                keyword=item
                index = current_index
            else:
                index = current_index
                keyword=item
                token = Token(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=TokenType.tk_minus
                    )
        elif item=="=":
            if keyword not in operator and keyword!="":
                token = Token(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=token_type(keyword)
                    )
                tokens.append(token)
                token = Token()
                keyword = ""
            if keyword=="=":
                keyword="=="
                token = Token(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=TokenType.tk_equal
                    )
            elif next(current_index,code)=="=":
                keyword=item
                index = current_index
            elif keyword=="" and next(current_index,code)!="=":
                keyword=item
                token = Token(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=TokenType.tk_assign
                    )
            else:
                token = equal(
                        keyword=keyword, index=index, line=line, tab=tab
                    )
                

        elif item=="/":
            if keyword!="" and keyword!="/":
                token = Token(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=token_type(keyword)
                    )
                tokens.append(token)
                token = Token()
                keyword = ""
            if keyword=="/":
                if next(current_index,code)=="=":
                    keyword="//"
                else:
                    token = Token(
                            keyword="//", index=index, line=line, tab=tab, tk_type=TokenType.tk_floor
                        )
            elif next(current_index,code)=="/" or  next(current_index,code)=="=" :
                keyword=item
                index = current_index
            else:
                index = current_index
                keyword=item
                token = Token(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=TokenType.tk_divide
                    )
        elif item=="^":
            if keyword!="":
                token = Token(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=token_type(keyword)
                    )
                tokens.append(token)
                token = Token()
                keyword = ""
            if next(current_index,code)=="=":
                keyword=item
                index = current_index
            else:
                index = current_index
                keyword=item
                token = Token(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=TokenType.tk_xor
                    )
        elif item=="%":
            if keyword!="":
                token = Token(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=token_type(keyword)
                    )
                tokens.append(token)
                token = Token()
                keyword = ""
            if next(current_index,code)=="=":
                keyword=item
                index = current_index
            else:
                index = current_index
                keyword=item
                token = Token(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=TokenType.tk_modulo
                    )
        elif item=="&":
            if keyword!="":
                token = Token(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=token_type(keyword)
                    )
                tokens.append(token)
                token = Token()
                keyword = ""
            if next(current_index,code)=="=":
                keyword=item
                index = current_index
            else:
                index = current_index
                keyword=item
                token = Token(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=TokenType.tk_ampersand
                    )
        elif item=="|":
            if keyword!="":
                token = Token(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=token_type(keyword)
                    )
                tokens.append(token)
                token = Token()
                keyword = ""
            if next(current_index,code)=="=":
                keyword=item
                index = current_index
            else:
                index = current_index
                keyword=item
                token = Token(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=TokenType.tk_bit_or
                    )
        elif item=="*":
            if keyword!="" and keyword!="*":
                token = Token(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=token_type(keyword)
                    )
                tokens.append(token)
                token = Token()
                keyword = ""
            
            if (next(current_index,code)=="*" or next(current_index,code)=="=") and (tokens[-1].tk_type==TokenType.tk_identifier or tokens[-1].tk_type==TokenType.tk_decimal or tokens[-1].tk_type==TokenType.tk_integer or next(current_index,code)=="="):
                keyword="*"
                index=current_index
            elif keyword=="*":
                token = Token(
                        keyword="**", index=index, line=line, tab=tab, tk_type=TokenType.tk_exponent
                )
            else:
                index = current_index
                keyword=item
                token = Token(
                    keyword=keyword, index=index, line=line , tab=tab,tk_type=TokenType.tk_asterisk
                )
            

        elif item=="~":
            if keyword!="":
                token = Token(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=token_type(keyword)
                    )
                tokens.append(token)
                token = Token()
                keyword = ""
            index = current_index
            keyword=item
            token = Token(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=TokenType.tk_bit_not
                    )
        elif item=="!":
            if keyword!="":
                token = Token(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=token_type(keyword)
                    )
                tokens.append(token)
                token = Token()
                keyword = ""
            index = current_index
            keyword=item

        elif item==":":
            if keyword!="":
                token = Token(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=token_type(keyword)
                    )
                tokens.append(token)
                token = Token()
                keyword = ""
            index = current_index
            keyword=item
            token = Token(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=TokenType.tk_colon
                    )
            
        elif item==",":
            if keyword!="":
                token = Token(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=token_type(keyword)
                    )
                tokens.append(token)
                token = Token()
                keyword = ""
            index = current_index
            keyword=item
            token = Token(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=TokenType.tk_comma
                    )

        else:
            if keyword=="":
                index=current_index
            keyword+= item


        if token.keyword != "":
            tokens.append(token)
            token = Token()
            keyword = ""
    if keyword!="":
        tokens.append(Token(
                        keyword=keyword, index=index, line=line, tab=tab, tk_type=token_type(keyword)
                    ))

    if is_string==True or is_list_dictionary_cpp_string==True:
        error=error_list.message(statement, line, current_index+1-last_line, file_name,expected=string_starter)
        error.string()
        return None
    elif is_dictionary==True:
        error=error_list.message(statement, line, current_index+1-last_line,file_name,expected="}")
        error.dictionary()
        return None
    elif is_cpp==True:
        error=error_list.message(statement, line, current_index+1-last_line,file_name)
        error.cpp()
        return None
    elif is_array==True:
        error=error_list.message(statement, line, current_index+1-last_line, file_name,expected="]")
        error.array()
        return None
    else:
        return tokens
