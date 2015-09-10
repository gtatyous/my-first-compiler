%{
#include "myscanner.h"
#include <iostream>
#include <fstream>
int char_count = 0, word_count = 0, line_count=0;
%}

%option noyywrap
%option yylineno

word [^ \t\n]+
eol \n
comment #.*

%%
"val"                                                 return TYPE;
"char"                                                return TYPE;
"string"                                              return TYPE;
"print"                                               return COMMAND_PRINT;
"random"                                              return COMMAND_RANDOM;
[a-zA-Z_][a-zA-Z0-9_]*                                return ID;
[+\-]?[0-9]+(\.0-9+)?([eE][+\-]?[0-9]+)?              return VAL_LITERAL;
eol                                                   ;
comment                                               ;
.                                                     return UNKNOWN;
%%

main() {
  FlexLexer* lexer = new yyFlexLexer();
  while (lexer->yylex());
  //print char, word, line count here
}
