import glob
import os
file=glob.glob('./tests/*.sw')
print("This program will compile the swallow compiler and run some tests")
print("Compiling the swallow compiler")
os.system("v swallow.v")
for item in file:
    print(f"\n\nCompiling {item} and running it\n\n") 
    os.system(f"./swallow compile {item} -o output")
    os.system("./output")