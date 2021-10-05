import glob
import os
file=glob.glob('./tests/*.sw')
print("This program will compile the swallow compiler and run some tests")
print("Compiling the swallow compiler")
# os.system("v swallow.v -o swallow")
for item in file:
    print(f"\n\nCompiling {item} and running it\n\n") 
    os.system(f"swallow compile {item} -emit-c")
    os.system("gcc ./temp.c -o output.exe")
    os.system("output")   