.cc.o:
	g++ -c $< -o $*.o

default: all

all: tube4

tube4.tab.cc: myparser.y
	bison -o tube4.tab.cc -b tube4 -d myparser.y

tube4.yy.cc: scanner.lex
	flex -o tube4.yy.cc scanner.lex

tube4.SymbolTree: ../DataStructures/SymbolTable.cpp
	g++ -c ../DataStructures/SymbolTable.cpp -o tube4.SymbolTree.o

tube4.SyntaxTree: ../DataStructures/SyntaxTree.cpp
	g++ -c ../DataStructures/SyntaxTree.cpp -o tube4.SyntaxTree.o

tube4: tube4.tab.o tube4.yy.o tube4.SymbolTree tube4.SyntaxTree
	g++ -o tube4 tube4.SyntaxTree.o tube4.SymbolTree.o tube4.tab.o tube4.yy.o -ll -ly

clean:
	rm -f tube4 tube4.tab.cc tube4.tab.hh tube4.yy.cc tube4.tab.o *~ *.o
