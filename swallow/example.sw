@free
def free_test() (str):
    str variable="some string"
    Ccode variable = inputString(stdin, 10); Ccode
    return variable  
def useless_func():
    pass
def main():
<<<<<<< HEAD
    print("Enter a string ")
    str a=input()
    colorprint("BLUE","BLUE")
    colorprint("BLUE","RED")
    printf("%f", 58.8//10)
    print("\n{a} {a} {a} {a}")
    printf( "\n Returned text %s\n", something())
=======
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
>>>>>>> 2b480f5718d0de00f129c0f77918900649a7b546
