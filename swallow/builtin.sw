#Builtin swallow function
def print(query):
    PRINT "$query\n"
def input(query):
    PRINT query
    out=INPUT
    PRINT "\n"
    return out