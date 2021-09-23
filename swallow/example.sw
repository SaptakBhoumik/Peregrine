def main():
    int a=2
    match a:
        case 4:
            print("It is 4\n")
            print("It is 4\n")
        case 2:
            print("It is 2\n")
            break
        default:
            print("I dont know what it is\n")
            break
    print("Who are you ")
    x=input()
    if x=="saptak" or x=="Saptak":
        print("Hi")
    elif a==2:
        print("bye")