Thank you for taking an interest in Peregrine.

Since it is a free software, you can contribute to make it better. New contributors are always welcome, whether you write code, create resources, report bugs, or suggest features.

The Peregrine is written primarily in C++

https://github.com/peregrine-lang/Peregrine/tree/main/Peregrine/lexer

This folder contains the tokenizer

The tokenizer breaks the source code into smaller parts

https://github.com/peregrine-lang/Peregrine/tree/main/Peregrine/parser

This is the parser and it generates the abstract syntax tree

https://github.com/peregrine-lang/Peregrine/tree/main/Peregrine/codegen

This folder contains the file that converts the abstract syntax tree to C++ or js code 

https://github.com/peregrine-lang/Peregrine/tree/main/Peregrine/docgen

This folder contains the file that converts the abstract syntax tree to html docs

https://github.com/peregrine-lang/Peregrine/blob/main/Peregrine/main.cpp

This is the entry point of the program

Follow the instruction in the readme to build it.

If you are making changes to the parser/lexer/analyzer then test it by running ``./peregrine.elf -dev_debug``. This should print the content of ![test.pe](https://github.com/peregrine-lang/Peregrine/blob/main/Peregrine/test.pe)(add your tests to test.pe) after generating the ast then converting it to the output shown on the console.

If you are making changes to the codegen then run ``./peregrine.elf compile can_comp.pe ``(for c++ backend) or ``./peregrine.elf compile can_comp.js.pe -js``(for js backend)
