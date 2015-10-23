.cc.o:
	g++ -c $< -o $*.o

default: all

all: tube5

tube5.tab.cc: myparser.y
	bison -o tube5.tab.cc -b tube5 -d myparser.y

tube5.yy.cc: scanner.lex
	flex -o tube5.yy.cc scanner.lex

tube5.SymbolTree: ../DataStructures/SymbolTable.cpp
	g++ -c ../DataStructures/SymbolTable.cpp -o tube5.SymbolTree.o

tube5.SyntaxTree: ../DataStructures/SyntaxTree.cpp
	g++ -c ../DataStructures/SyntaxTree.cpp -o tube5.SyntaxTree.o

tube5: tube5.tab.o tube5.yy.o tube5.SymbolTree tube5.SyntaxTree
	g++ -o tube5 tube5.SyntaxTree.o tube5.SymbolTree.o tube5.tab.o tube5.yy.o -ll -ly

clean:
	rm -f tube5 tube5.tab.cc tube5.tab.hh tube5.yy.cc tube5.tab.o *~ *.o
