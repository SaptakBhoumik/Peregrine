#Builtin swallow function
def print(query):
    text=f"{query}\n"
    PRINT text
def input(query):
    PRINT query
    out=INPUT
    PRINT "\n"
    return out