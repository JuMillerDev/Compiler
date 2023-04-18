all: compile clean

compile: parser lexer
	g++ -std=c++14 -o kompilator parser.tab.cpp lexer.c variables/variable.cpp value/value.cpp expressions/expression.cpp commands/command.cpp commands/commands.cpp conditions/condition.cpp assembly/assembly.cpp

parser: parser.ypp
	bison -b parser -d parser.ypp

lexer: lexer.l
	flex -o lexer.c lexer.l

clean:
	rm -f parser.tab.* lexer.c *.o