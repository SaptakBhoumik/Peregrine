#Builtin swallow function
#Author: Saptak Bhoumik
#Builtin.h is imported by default
def print(str query):
    Ccode printf("%s",query);  Ccode
def colorprint(str string, str flags):
    Ccode _colorprint(string,flags,True); Ccode
@free
def input() ->str:
    str variable="some string"
    Ccode variable = inputString(stdin, 10); Ccode
    return variable
def panic(str string):
    colorprint(string,"RED")
    exit(0)