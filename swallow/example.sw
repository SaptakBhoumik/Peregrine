def main():
    int a=0
    int c=0
    int v=0
    int i=2
    while True:
        if a>1:
            while i<=a/2:
                v=a%i
                if v==0:
                    break
                i++
        if v!=0 and a>1:
            c+=a
        a++
        if a==1000000:
            break