from .lexDS import *

def enter_state(current_index: int, item: str, context: LexContext, funcs: LexFuncs):
   context.token = Token() # make sure token is a new one

def during_state(current_index: int, item: str, context: LexContext, funcs: LexFuncs):
    # check whether the current character triggers a state switch

    # state checking
    if item == '"' or item == "'":
            funcs.tokenize(context)
            context.str_starter = item
            return funcs.change_state(context, "string")
    
    # cache the text
    funcs.cache_token(context, item, current_index)