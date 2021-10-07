import glob
import os
from sys import platform
ext = ""
sign = "/"
if platform == "win32":
    ext = ".exe"
    sign = "\\"
file = glob.glob(f'.{sign}tests{sign}*.pe')
print("This program will compile the peregrine compiler and run some tests")
print("Compiling the peregrine compiler")
os.system(f"v peregrine.v -o peregrine{ext}")
print("compiling and running tests...")
for item in file:
    print(f"\n\nCompiling {item} and running it\n\n")
    os.system(f".{sign}peregrine{ext} compile {item} -emit-c")
    os.system(f"gcc temp.c -o output{ext}")
    os.system(f".{sign}output{ext}")
