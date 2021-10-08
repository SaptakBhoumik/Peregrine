
<img id="banner" src="./graphics/banner.png" style="width: 800px; align: center;"/>

# Peregrine

Peregrine is a compiled programming language currently under development. Docs coming soon.

![-----------------------------------------------------](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/rainbow.png)

# Why am I creating this language?

I am creating it as a fast alternative to Python and an easy one to C.

![-----------------------------------------------------](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/rainbow.png)

# Goals

- Ease of use
- Fast
- Highly modular
- Occupies less memory
- Compiled


![-----------------------------------------------------](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/rainbow.png)

# How does it work?

You can consider it to be a superset of C which compiles to clean C. So valid c code is also valid Peregrine code (the language is named Peregrine). It will have no garbage collector because it is a system programming language but it will be very easy to use so there will be less chance of memory leak.

![-----------------------------------------------------](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/rainbow.png)

# Requirements to build from source
 - [Gcc compiler](https://gcc.gnu.org/)
 - [V compiler](https://vlang.io)

# Build instruction
 
- Clone this repository using the following command -> `git clone https://github.com/Peregrine-lang/Peregrine.git`
- Run `cd Peregrine/Peregrine`
- To build it run `v peregrine.v`
- That's it


![-----------------------------------------------------](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/rainbow.png)

# Primary implementation language

It is [written in V](https://vlang.io) 
![-----------------------------------------------------](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/rainbow.png)

# Progress

Currently we are working on the codegen and some Peregrine programs are working :) 

![-----------------------------------------------------](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/rainbow.png)

# Example

Here is a small example for you to understand:
```py
def main():
    print("Hello, World!")
```
The `main` function is the entry point for the program.

![-----------------------------------------------------](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/rainbow.png)

# Have more questions?

Cool, you can contact me via mail.
<br> Email: saptakbhoumik@gmail.com

![-----------------------------------------------------](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/rainbow.png)

# Want to contribute?

Great, go ahead and make the changes you want. 


Have a look at the [open issues](https://github.com/Peregrine-lang/Peregrine/issues) to find a mission that resonates with you.

Please check [CONTRIBUTING.md](https://github.com/Peregrine-lang/Peregrine/blob/main/CONTRIBUTING.md) to know how you can contribute.

If you create any new file make sure to comment your name in the file as shown   `Original author: Your name`

![-----------------------------------------------------](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/rainbow.png)

# Testing 
The project has automatic tests which will run on any pull reuest and push to the project.   
Tests which require user input wont run but they will still be compiled.  
If you add new functionality to the project please create a test for it.  
### Its recommended to only use input when absolutly necessary (example: you test the `input()` function)
This is the case because tests which require user input will only be compiled and can only be checked for syntax errors not issues with the functionality of the given test subject.
### `/Peregrine/tests/ci` - Tests which require no user input.
### `/Peregrine/tests/manual` - Tests which require user input - **use only if absolutely necessary**


![-----------------------------------------------------](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/rainbow.png)
# License

The Peregrine compiler is licensed under the [Mozilla Public License](https://github.com/Peregrine-lang/Peregrine/blob/main/LICENSE)
