tk_plus: int = 0  # +
tk_minus: int = 1  # -
tk_divide: int = 2  # /
tk_asterisk: int = 3  # *
tk_xor: int = 4  # ^
tk_modulo: int = 5  # %
tk_greater: int = 6  # >
tk_less: int = 7  # <
tk_bit_and: int = 8  # &
tk_bit_or: int = 9  # |
tk_bit_not: int = 10  # ~
tk_assign: int = 11  # =
tk_excl: int = 12  # !
tk_colon: int = 13  # :
tk_dot: int = 14  # .
tk_l_bracket: int = 15  # {
tk_r_bracket: int = 16  # }
tk_lsq_bracket: int = 17  # [
tk_rsq_bracket: int = 18  # ]
tk_l_paren: int = 19  # (
tk_r_paren: int = 20  # )
tk_hashtag: int = 21  # #
tk_comma: int = 22  # ,
tk_sin_quote: int = 23  # '
tk_db_quote: int = 24  # "

tk_equal: int = 25  # ==
tk_not_equal: int = 26  # !=
tk_db_slash: int = 27  # /
tk_gr_or_equ: int = 28  # >=
tk_increment: int = 29  # ++
tk_less_or_equ: int = 30  # <=
tk_decrement: int = 31  # --
tk_arrow: int = 32  # ->
tk_shift_right: int = 33  # >>
tk_shift_left: int = 34  # <<
tk_plus_equal: int = 35  # +=
tk_minus_equal: int = 36  # -=
tk_times_equal: int = 37  # *=
tk_slash_equal: int = 38  # /=
tk_mod_equal: int = 39  # %=
tk_shift_right_equal: int = 40  # >>=
tk_shift_left_equal: int = 41  # <<=
tk_bit_and_equal: int = 42  # &=
tk_bit_or_equal: int = 43  # |=
tk_bit_xor_equal: int = 44  # ^=

tk_identifier: int = 45  # foo, bar
tk_integer: int = 46  # 123, 65
# keywords tokens
tk_true: int = 47  # true
tk_false: int = 48  # false
tk_none: int = 49  # none
tk_const: int = 50  # const
tk_import: int = 51  # import
tk_if: int = 52  # if
tk_else: int = 53  # else
tk_elif: int = 54  # elif
tk_while: int = 55  # while
tk_for: int = 56  # for
tk_break: int = 57  # break
tk_continue: int = 58  # continue
tk_match: int = 59  # match
tk_def: int = 60  # def
tk_pass: int = 61  # pass
tk_return: int = 62  # return
tk_and: int = 63  # and
tk_or: int = 64  # or
tk_not: int = 65  # not
tk_is: int = 66  # is
tk_in: int = 67  # in
tk_try: int = 68  # try
tk_catch: int = 69  # catch
tk_raise: int = 70  # raise
tk_finally: int = 71  # finally
# types
string: int = 72
integer: int = 73
decimal: int = 74
dictionary: int = 75
array: int = 76
