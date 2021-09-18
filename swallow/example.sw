@free
def free_test() (str):
    str variable="some string"
    Ccode variable = inputString(stdin, 10); Ccode
    return variable  
def useless_func():
    pass
def main():
    colorprint("If you use @free before a function then whatever it will return will be freed at the end. Now enter some text:-  ","BLUE")
    a=free_test()
    printf("You entered %s\n",a)
    str x="If you want you can specify the type of variable \n"
    print(x)
    y="But if you dont then also it will work \n"
    print(y)
    j=7
    printf("Like %g \n",j)
    print("Also pass is working now")
    useless_func()