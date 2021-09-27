def main():
    int arg1=45
    int arg2=50
    int add=0
    #__asm__ ( "addl %%ebx, %%eax;" : "=a" (add) : "a" (arg1) , "b" (arg2) )
    #printf("%lld",add)