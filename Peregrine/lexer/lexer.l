%{ 

	#include <stdio.h>
	#include <stdlib.h>
    #include "./lexer/lexer.h"
%}

%%
"+" return tk_plus;
"-" return tk_minus;
"/" return tk_divide;
"*" return tk_asterisk;
"^" return tk_hat;
"%" return tk_modulo;
">" return tk_greater;
"<" return tk_less;
"&" return tk_bit_and;
"|" return tk_bit_or;
"~" return tk_bit_not;
"=" return tk_assign;
"!" return tk_excl;
":" return tk_colon;
"." return tk_dot;
"{" return tk_l_bracket;
"}" return tk_r_bracket;
"[" return tk_lsq_bracket;
"]" return tk_rsq_bracket;
"(" return tk_l_paren;
")" return tk_r_paren;
"'" return tk_sin_quote;//left here
"," return tk_db_quote;
"/" return tk_divide;
"/" return tk_divide;
"/" return tk_divide;
"/" return tk_divide;
"/" return tk_divide;
"/" return tk_divide;
"/" return tk_divide;
"/" return tk_divide;
"/" return tk_divide;
"/" return tk_divide;
"/" return tk_divide;
"/" return tk_divide;
"/" return tk_divide;
"/" return tk_divide;
"/" return tk_divide;
"/" return tk_divide;
"/" return tk_divide;
"/" return tk_divide;
"/" return tk_divide;
"/" return tk_divide;
"/" return tk_divide;
"/" return tk_divide;
"/" return tk_divide;
"/" return tk_divide;
"/" return tk_divide;
"/" return tk_divide;
"/" return tk_divide;
"/" return tk_divide;
"/" return tk_divide;
"/" return tk_divide;
"/" return tk_divide;
"/" return tk_divide;
"/" return tk_divide;
"/" return tk_divide;
"/" return tk_divide;
"/" return tk_divide;
"/" return tk_divide;
"/" return tk_divide;
"/" return tk_divide;
"/" return tk_divide;
"/" return tk_divide;
"/" return tk_divide;
"/" return tk_divide;
"/" return tk_divide;
"/" return tk_divide;
"/" return tk_divide;
"/" return tk_divide;
"/" return tk_divide;
"/" return tk_divide;
"/" return tk_divide;
"/" return tk_divide;
"/" return tk_divide;
"/" return tk_divide;
"/" return tk_divide;
"/" return tk_divide;
"/" return tk_divide;
"/" return tk_divide;
"/" return tk_divide;
"/" return tk_divide;


[0-9]+ return tk_integer;
[_a-zA-Z][_a-zA-Z0-9]* return tk_identifier ;

%%