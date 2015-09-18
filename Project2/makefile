.cc.o:
	g++ -c $< -o $*.o

default: all

all: myparser

myparser.tab.cc: myparser.y
	bison -o myparser.tab.cc -b myparser -d myparser.y

scanner.yy.cc: scanner.lex
	flex -o scanner.yy.cc scanner.lex

myparser: myparser.tab.o scanner.yy.o
	g++ -o myparser myparser.tab.o scanner.yy.o -ll -ly

clean:
	rm -f myparser.tab.cc myparser.tab.hh scanner.yy.cc myparser.tab.o *~ *.o
