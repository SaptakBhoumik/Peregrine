<img id="banner" src="./graphics/banner.png" style="width: 800px; align: center;"/>

# Peregrine
![GitHub Repo stars](https://img.shields.io/github/stars/peregrine-lang/peregrine?style=social)
![GitHub issues](https://img.shields.io/github/issues/peregrine-lang/peregrine?color=green&style=flat-square)
![GitHub contributors](https://img.shields.io/github/contributors/peregrine-lang/peregrine?style=flat-square)
   
Peregrine is a compiled programming language currently under development. Documentation is coming soon.

![-----------------------------------------------------](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/rainbow.png)

## Purpose and intent

![-----------------------------------------------------](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/rainbow.png)

### Why am I creating this language?

I am creating it to have the simplicity of Python with the efficiency of C.

![-----------------------------------------------------](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/rainbow.png)

### Goals

- Ease of use
- Fast
- Highly modular
- Occupies less memory
- Compiled

![-----------------------------------------------------](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/rainbow.png)

### How does it work?

You can consider it to be a dialect of python which compiles to clean C++. It will have no garbage collector because it is a system programming language.


![-----------------------------------------------------](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/rainbow.png)

## Building/compiling Peregrine

![-----------------------------------------------------](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/rainbow.png)

### Requirements to build from source

- [G++ compiler](https://gcc.gnu.org/)

# Compilation instructions

**1)Clone the rewrite branch of https://github.com/peregrine-lang/Peregrine.git**
```bash
git clone -b rewrite https://github.com/peregrine-lang/Peregrine.git
```
**2)Cd into the directory**
```bash 
cd Peregrine
```
**3)Build it**
```bash
meson builddir
cd builddir
ninja
```
This will create the binary of the compiler named ``./peregrine.elf`` in the builddir folder

**C++ backend**
To compile it using the c++ backing just run ``./peregrine.elf compile path_to_file.pe`` .It will create the executable named ``./a.out``. Run it to see the result. Check the [can_comp.pe](https://github.com/peregrine-lang/Peregrine/blob/rewrite/can_comp.pe) file in the root directory to know what you can do with the c++ backend at this point 

**JS Backend**
To use the javascript backend use the following command
``./peregrine.elf compile path_to_file.js.pe -js``.
It will create the javascript file named ``index.js``. Run the generated javascript using ``node index.js``. Check the [can_comp.js.pe](https://github.com/peregrine-lang/Peregrine/blob/rewrite/can_comp.js.pe) file in the root directory to know what you can do with the js backend at this point

![-----------------------------------------------------](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/rainbow.png)

## Primary implementation language

It is written in c++
![-----------------------------------------------------](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/rainbow.png)

## Progress

Currently we are working on the codegen and some Peregrine programs are working

![-----------------------------------------------------](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/rainbow.png)

## Example

Here is a small example for you to understand:

```py
def main():
    print("Hello, World!")
```

The `main` function is the entry point for the program.

![-----------------------------------------------------](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/rainbow.png)

## Have questions?

Cool, you can contact me via mail.
<br> Email: saptakbhoumik@gmail.com
<br> Discord : https://discord.gg/CAMgzwDJDM

![-----------------------------------------------------](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/rainbow.png)

## Want to contribute?

Great, go ahead and make the changes you want, then submit a new pull request

Take a look at the [open issues](https://github.com/Peregrine-lang/Peregrine/issues) to find a mission that resonates with you.

Please check [CONTRIBUTING.md](https://github.com/Peregrine-lang/Peregrine/blob/main/CONTRIBUTING.md) to learn how you can contribute.


![-----------------------------------------------------](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/rainbow.png)


# License

The Peregrine compiler is licensed under the [Mozilla Public License](https://github.com/Peregrine-lang/Peregrine/blob/main/LICENSE), which is attached in this repository
