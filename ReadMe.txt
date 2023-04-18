/*
* Kod kompilatora do projektu z JFTT 2022
*
* Autor: Yuliia Melnyk
* nr. indeksu: 246202
*/


-----------------------------------------------
Narzędzia:

bison (GNU Bison) 3.8.2
flex 2.6.4
GNU Make 4.3
g++ 11.3.0

libcln-dev 1.3.6
-----------------------------------------------
Pliki:

ReadMe.txt
Makefile

parser.ypp
lexer.l

variable.cpp
variable.hpp
value.cpp
value.hpp
expression.cpp
expression.hpp
condition.cpp
condition.hpp
command.cpp
command.hpp
commands.cpp
commands.hpp
assembly.cpp
assembly.hpp
-----------------------------------------------
Wywolanie programu:

1) make
2) ./kompilator <plik wejsciowy> <plik wyjsciowy>


