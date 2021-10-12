from .lexDS import *

def enter_state(current_index: int, item: str, context: LexContext, funcs: LexFuncs):
    context.token = Token() # make sure token is new
    funcs.cache_token(context, context.str_starter, current_index - 1, TokenType.tk_str) # cache the first " or '


def during_state(current_index: int, item: str, context: LexContext, funcs: LexFuncs):
    # check whether the current character triggers a state switch

    # state checking
    if item == context.str_starter:
        funcs.cache_token(context, item, current_index, TokenType.tk_str) # cache the last " or '
        funcs.tokenize(context)
        context.str_starter = ""
        return funcs.change_state(context, "default")
    
    # cache the text
    funcs.cache_token(context, item, current_index, TokenType.tk_str)