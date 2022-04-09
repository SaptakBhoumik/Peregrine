<p align='center'>
   <img id="banner" src="./graphics/banner.png" style="width: 65%; align: center;"/>
   <br/>
   <i>A Blazing-Fast Language for the Blazing-Fast world.</i>
   <br/><br/>
   <a href="https://discord.gg/CAMgzwDJDM">
      <img src="https://img.shields.io/discord/895990298419818546?logo=discord?style=social">
   </a>
   <img src="https://img.shields.io/github/stars/peregrine-lang/peregrine?style=social">
   <br/>
   <img src="https://img.shields.io/github/issues/peregrine-lang/peregrine?color=green">
   <img src="https://img.shields.io/github/contributors/peregrine-lang/peregrine">
   <br/>
   <img src="https://img.shields.io/github/workflow/status/peregrine-lang/peregrine/Build%20-%20Meson">
   <br/>
   <h1>The Peregrine Programming Language</h1>
</p>

Peregrine is a Compiled, Systems Programming Language, currently under development.


## Purpose and intent


### Why am I creating this language?

I am creating it to have the simplicity of Python with the efficiency of C.

### Goals

- Ease of use
- Fast
- Highly modular
- Occupies less memory
- Compiled


### How does it work?

You can consider it to be a dialect of python which compiles to clean C++. It will have no garbage collector because it is a system programming language.


## Building/compiling Peregrine


### Requirements to build from source

- [clang compiler](https://clang.llvm.org/)

# Compilation instructions

**1) Clone the main branch of https://github.com/peregrine-lang/Peregrine.git**
```bash
git clone https://github.com/peregrine-lang/Peregrine.git
```
**2) Cd into the directory**
```bash
cd Peregrine
```
**3) Build it**
```bash
meson builddir
cd builddir
ninja
```
This will create the binary of the compiler named ``./peregrine.elf`` in the builddir folder

**C++ backend**
To compile it using the c++ backing just run ``./peregrine.elf compile path_to_file.pe`` .It will create the executable named ``./path_to_file``. Run it to see the result. Check the [can_comp.pe](https://github.com/peregrine-lang/Peregrine/blob/rewrite/can_comp.pe) file in the root directory to know what you can do with the c++ backend at this point

**JS Backend**
To use the javascript backend use the following command
``./peregrine.elf compile path_to_file.js.pe -js``.
It will create the javascript file named ``path_to_file.js``. Run the generated javascript using ``node path_to_file.js``. Check the [can_comp.js.pe](https://github.com/peregrine-lang/Peregrine/blob/rewrite/can_comp.js.pe) file in the root directory to know what you can do with the js backend at this point


## Progress

Currently we are working on the codegen and some Peregrine programs are working


## Example

Here is a small example for you to understand:

```py
def fib(n:int) -> int :
    if n <= 0:
        return 1
    return fib(n-1) + fib(n-2)

def main():
    count:int = 0
    res:int = 0

    while count < 40:
        res = fib(count)
        count++

```

The `main` function is the entry point for the program.


## Have questions?

Cool, you can contact me via mail.
<br> Email: saptakbhoumik@gmail.com
<br> Discord : https://discord.gg/CAMgzwDJDM



## Want to contribute?

Great, go ahead and make the changes you want, then submit a new pull request

Take a look at the [open issues](https://github.com/Peregrine-lang/Peregrine/issues) to find a mission that resonates with you.

Please check [CONTRIBUTING.md](https://github.com/Peregrine-lang/Peregrine/blob/main/CONTRIBUTING.md) to learn how you can contribute.



# License

The Peregrine compiler is licensed under the [Mozilla Public License](https://github.com/Peregrine-lang/Peregrine/blob/main/LICENSE), which is attached in this repository
