.cc.o:
	g++ -c $< -o $*.o

default: all

all: tube3

tube3.tab.cc: myparser.y
	bison -o tube3.tab.cc -b tube3 -d myparser.y

tube3.yy.cc: scanner.lex
	flex -o tube3.yy.cc scanner.lex

tube3.SymbolTree: ../DataStructures/SymbolTable.cpp
	g++ -c ../DataStructures/SymbolTable.cpp -o tube3.SymbolTree.o

tube3.SyntaxTree: ../DataStructures/SyntaxTree.cpp
	g++ -c ../DataStructures/SyntaxTree.cpp -o tube3.SyntaxTree.o

tube3: tube3.tab.o tube3.yy.o tube3.SymbolTree tube3.SyntaxTree
	g++ -o tube3 tube3.SyntaxTree.o tube3.SymbolTree.o tube3.tab.o tube3.yy.o -ll -ly

clean:
	rm -f tube3 tube3.tab.cc tube3.tab.hh tube3.yy.cc tube3.tab.o *~ *.o
