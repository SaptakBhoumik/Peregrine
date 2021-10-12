"""
*  
*  Copyright (c) Peregrine-lang, 2021. All rights reserved.
*
"""

from enum import IntEnum, auto
from dataclasses import dataclass

@dataclass
class Token:  # change class to struct
    keyword: str = ""
    index: int = 0
    line: int = 1
    tk_type: int = 0
    tab: float = 0

class TokenType(IntEnum):
	tk_plus = auto()       # + #done
	tk_minus = auto()     # -	#done
	tk_divide = auto()     # / #done
	tk_asterisk = auto()    # *
	tk_xor = auto()        # ^ #done
	tk_modulo = auto()     # % #done
	tk_greater = auto()    # > #done
	tk_less = auto()       # < #done
	tk_ampersand = auto()    # & #done
	tk_bit_or = auto()     # | #done
	tk_bit_not = auto()    # ~ #done
	tk_assign = auto()     # = #done
	tk_excl = auto()        # ! #done
	tk_colon = auto()      # : #done
	tk_dot = auto()        # . 
	tk_l_paren = auto()     # ( #done
	tk_r_paren = auto()     # ) #done
	tk_hashtag = auto()     # #
	tk_comma = auto()      # , #done

	tk_equal = auto()             # == #done
	tk_not_equal = auto()         # != #done
	tk_floor = auto()             # // #done
	tk_gr_or_equ = auto()         # >= #done
	tk_less_or_equ = auto()       # <= #done
	tk_increment = auto()         # ++ #done
	tk_decrement = auto()         # --#done
	tk_arrow = auto()             # -> #done
	tk_shift_right = auto()       # >> #done
	tk_shift_left = auto()        # << #done
	tk_floor_equal = auto()       # //= #done
	tk_plus_equal = auto()        # += #done
	tk_minus_equal = auto()        # -= #done
	tk_times_equal = auto()       # *= #done
	tk_slash_equal = auto()       # /= #done
	tk_mod_equal = auto()         # %= #done
	tk_shift_right_equal = auto()  # >>= #done
	tk_shift_left_equal = auto()   # <<= #done
	tk_bit_and_equal = auto()     # &= #done
	tk_bit_or_equal = auto()       # |= #done
	tk_bit_xor_equal = auto()      # ^= #done
	
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
	tk_try = auto()           # try
	tk_catch = auto()         # catch
	tk_raise = auto()         # raise
	tk_finally = auto()       # finally
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
	default = auto()
	integer = auto()
	array = auto() #done
	dictionary = auto() #done
	string = auto() #done
	tk_ident = auto()         # beginning of identation
	tk_dedent = auto()        # end of identation

def next(index:int, code:list)-> str:
	return "" if len(code) == index + 1 else code[index+1]