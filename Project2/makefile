.cc.o:
	g++ -c $< -o $*.o

default: all

all: tube2

tube2.tab.cc: myparser.y
	bison -o tube2.tab.cc -b tube2 -d myparser.y

tube2.yy.cc: scanner.lex
	flex -o tube2.yy.cc scanner.lex

tube2.dataStructs: ../DataStructures/SymbolTable.cpp
	g++ -c ../DataStructures/SymbolTable.cpp -o tube2.dataStructs.o

tube2: tube2.tab.o tube2.yy.o tube2.dataStructs
	g++ -o tube2 tube2.dataStructs.o tube2.tab.o tube2.yy.o -ll -ly

clean:
	rm -f tube2 tube2.tab.cc tube2.tab.hh tube2.yy.cc tube2.tab.o *~ *.o
