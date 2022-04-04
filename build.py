import os
from Peregrine import all_src as files
from os import path
def system(cmd):
    if(os.system(cmd)!=0):
        raise Exception("Command failed: "+cmd)
import platform
bin_name="peregrine"
if platform.system()=="Windows":
    bin_name+=".exe"

cpp_compiler="clang++"#change it if you want to use another compiler
cpp_std="-std=c++17"
cpp_args=["-IPeregrine/" ,"-I../Peregrine/" ,cpp_std]
libs=[]
obj=[]
buildir="builddir"
system(f"mkdir -p {buildir}")
modified=False
cargs=""
for x in cpp_args:
    cargs+=" "+x
for x in files:
    x="Peregrine/"+x
    obj_name=x.replace("/","_").replace(".cpp",".o")
    obj.append(obj_name)
    if path.exists(f"{buildir}/{obj_name}"):
        if path.getmtime(x)>path.getmtime(f"{buildir}/"+obj_name):
            modified=True
            system(f"{cpp_compiler} -c {cargs} {x} -o {buildir}/{obj_name}")
    else:
        modified=True
        system(f"{cpp_compiler} -c {cargs} {x} -o {buildir}/{obj_name}")
if modified==True:
    cmd=f"{cpp_compiler} {cargs}"
    for x in obj:
        cmd+=f" {buildir}/{x}"
    for x in libs:
        cmd+=f" -l{x}"
    cmd+=f" -o {buildir}/{bin_name}"
    system(cmd)
else:
    print("No changes detected, skipping build")
    