import glob
import os
from sys import platform
ext=""
if platform == "win32":
    ext=".exe"
file=glob.glob('./tests/*.pe')
print("This program will compile the peregrine compiler and run some tests")
print("Compiling the peregrine compiler")
os.system(f"v peregrine.v -o peregrine{ext}")
for item in file:
    print(f"\n\nCompiling {item} and running it\n\n") 
    os.system(f"peregrine compile {item} -emit-c")
    os.system(f"gcc ./temp.c -o output{ext}")
    os.system(f"output{ext}")   