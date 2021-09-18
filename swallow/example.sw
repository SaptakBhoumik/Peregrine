@free
def free_test()->str:
    str variable="some string"
    Ccode variable = inputString(stdin, 10); Ccode
    return variable  
def main():
    colorprint("You can use -> now. Now enter some text:-  ","BLUE")
    a=free_test()
    printf("You entered %s\n",a)