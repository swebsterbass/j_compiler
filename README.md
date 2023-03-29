# J Compiler

A small compiler that will transform code written in a stack oriented language, called J, into LC4 assembly code much the way that the lcc compiler converter converts C code into assembly. This compiler is designed to use exactly the same calling convention that lcc does that way my J programs can call subroutines compiled by lcc and vice versa.

This compiler was deveolped inside of a virtual machine that contains the clang compiler. There is a makefile for compiling all of my C source code to produce the final
executable program called `jc`. This program will act much like lcc does, when provided with a properly formatted source file in the J language it will produce the corresponding assembly code. That is

`>> ./jc foo.j`

will produce a new file called foo.asm if foo.j contains an acceptable program
otherwise it will print out helpful error messages.
