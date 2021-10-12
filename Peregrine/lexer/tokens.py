"""
*  
*  Copyright (c) Peregrine-lang, 2021. All rights reserved.
*
"""

from enum import IntEnum, auto

class TokenType(IntEnum):
	tk_plus = auto()       # + 
	tk_minus = auto()     # -	
	tk_divide = auto()     # / 
	tk_asterisk = auto()    # * 
	tk_xor = auto()        # ^ 
	tk_modulo = auto()     # % 
	tk_greater = auto()    # > 
	tk_less = auto()       # < 
	tk_ampersand = auto()    # & 
	tk_bit_or = auto()     # | 
	tk_bit_not = auto()    # ~ 
	tk_assign = auto()     # = 
	tk_excl = auto()        # ! 
	tk_colon = auto()      # : 
	tk_dot = auto()        # . 
	tk_l_paren = auto()     # ( 
	tk_r_paren = auto()     # ) 
	tk_hashtag = auto()     # # 
	tk_comma = auto()      # , 

	tk_exponent = auto() #**
	tk_equal = auto()             # == 
	tk_not_equal = auto()         # != 
	tk_floor = auto()             # // 
	tk_gr_or_equ = auto()         # >= 
	tk_less_or_equ = auto()       # <= 
	tk_increment = auto()         # ++ 
	tk_decrement = auto()         # --
	tk_arrow = auto()             # -> 
	tk_shift_right = auto()       # >> 
	tk_shift_left = auto()        # << 
	tk_floor_equal = auto()       # //= 
	tk_plus_equal = auto()        # += 
	tk_minus_equal = auto()        # -= 
	tk_times_equal = auto()       # *= 
	tk_slash_equal = auto()       # /= 
	tk_mod_equal = auto()         # %= 
	tk_shift_right_equal = auto()  # >>= 
	tk_shift_left_equal = auto()   # <<= 
	tk_bit_and_equal = auto()     # &= 
	tk_bit_or_equal = auto()       # |= 
	tk_bit_xor_equal = auto()      # ^= 
	
	tk_identifier = auto()  # foo, bar 

	# keywords tokens
	tk_true = auto()          # true 
	tk_false = auto()         # false 
	tk_none = auto()          # none 
	tk_const = auto()         # const 
	tk_import = auto()        # import 
	tk_if = auto()            # if 
	tk_else = auto()          # else 
	tk_elif = auto()          # elif 
	tk_while = auto()         # while 
	tk_for = auto()           # for 
	tk_break = auto()         # break  
	tk_continue = auto()      # continue 
	tk_match = auto()         # match 
	tk_case = auto()          # case 
	tk_default = auto()       # default 
	tk_def = auto()           # def 
	tk_pass = auto()          # pass 
	tk_return = auto()        # return 
	tk_and = auto()           # and 
	tk_or = auto()            # or 
	tk_not = auto()           # not 
	tk_is = auto()            # is 
	tk_in = auto()            # in 
	tk_ccode = auto()         # Ccode 
	tk_class = auto()         # class 
	tk_struct = auto()        # struct 

	# language types
	tk_str = auto() 
	tk_bool = auto() 
	tk_char = auto() 
	tk_float = auto() 
	tk_float32 = auto() 
	tk_void = auto() 
	tk_int = auto() 
	tk_int32 = auto() 
	tk_int16 = auto() 
	tk_int8 = auto() 
	tk_uint32 = auto() 
	tk_uint16 = auto() 
	tk_uint8 = auto() 
	tk_uint = auto() 
	#value type
	decimal = auto()  # done
	integer = auto() 
	array = auto() 
	dictionary = auto() 
	string = auto() 
	tk_ident = auto()         # beginning of identation
	tk_dedent = auto()        # end of identation

def next(index:int, code:list)-> str:
	if len(code)==index+1:
		return ""
	else:
		return code[index+1]