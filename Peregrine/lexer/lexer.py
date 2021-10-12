from states.lexDS import *
from states import default, string


def lex(code: str):
    # code is read char by char. And char can alter how the code is lexed (alter the state)
    # for instance, everything after a double quote is considered string and definitely not array, dict and etc.

    states = {
        "default" : LexState(default.enter_state, default.during_state),
        "string" : LexState(string.enter_state, string.during_state)
    }
    context = LexContext("default") # default state is 'default'

    # setup LexFuncs
    def change_state(context: LexContext, new_state:str):
        context.current_state = new_state
        context.state_just_changed = True
    def tokenize(context: LexContext):
        context.tokens.append(cp_token(context.token))
        context.token = Token() # reset token for next state
    def cache_token(context: LexContext, item: str = "", index: int = 0, tk_type: int = 0):
        if not context.is_caching_token:
            context.token.index = index
            context.token.line = context.line
            context.token.tk_type = tk_type
            context.token.tab = context.tab
        context.token.keyword += item
    funcs = LexFuncs(change_state, tokenize, cache_token)

    # start state machine
    for current_index, item in enumerate(code):
        if context.state_just_changed:
            states[context.current_state].enter_state(current_index, item, context, funcs)
            context.state_just_changed = False
        states[context.current_state].during_state(current_index, item, context, funcs)

    return context.tokens

print(lex("lasdf 'wow it is a string' asdfasdf 'wow another string'"))