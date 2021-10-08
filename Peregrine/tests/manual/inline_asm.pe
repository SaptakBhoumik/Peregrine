def main():
    int arg1=45
    int arg2=50
    int add=0
    print("It should add 45 and 50 using asm and print it\n")
    asm("addl %%ebx, %%eax;" : "=a" (add) : "a" (arg1) , "b" (arg2))
    printf("%lld",add)