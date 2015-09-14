%{
#include "tokens.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
int line_count=0, mytokens = 0;
%}

%option noyywrap
%option yylineno

%%
val|char|string                                             std::cout << "TYPE: " << yytext << std::endl;
print                                                       std::cout << "COMMAND_PRINT: " << yytext << std::endl;
random                                                      std::cout << "COMMAND_RANDOM: " << yytext << std::endl;
[a-zA-Z_][a-zA-Z_0-9]*                                      std::cout << "ID: " << yytext << std::endl;
([0-9]+(\.[0-9]+)?|(\.[0-9]+))([eE][+\-]?[0-9]+)?           std::cout << "VAL_LITERAL: " << yytext << std::endl;
\'[^'\n]?\'                                                 std::cout << "CHAR_LITERAL: " << yytext << std::endl;
\'[^'\n]{2,}\'                                              std::cout << "ERRRO(line " << ++line_count << "): Mutli char literal" << std::endl; exit(1); 
\'                                                          std::cout << "ERROR(line " << ++line_count << "): Uterminated char" << std::endl; exit(1);
\"[^"\n]*\"                                                 std::cout << "STRING_LITERAL: " << yytext << std::endl;
\"                                                          std::cout << "ERROR(line " << ++line_count << "): Unterminated string" << std::endl; exit(1);
[+\-*/()=,{}\[\]\.;]                                        std::cout << "ASCII_CHAR: " << yytext << std::endl;
\+=                                                         std::cout << "ASSIGN_ADD: " << yytext << std::endl;
-=                                                          std::cout << "ASSIGN_SUB: " << yytext << std::endl;
\*=                                                         std::cout << "ASSIGN_MULT: " << yytext << std::endl;
"/="                                                        std::cout << "ASSIGN_DIV: " << yytext << std::endl;
"=="                                                        std::cout << "COMP_EQU: " << yytext << std::endl;
"!="                                                        std::cout << "COMP_NEQU: " << yytext << std::endl;
"<"                                                         std::cout << "COMP_LESS: " << yytext << std::endl;
"<="                                                        std::cout << "COMP_LTE: " << yytext << std::endl;
">"                                                         std::cout << "COMP_GTR: " << yytext << std::endl;
">="                                                        std::cout << "COMP_GTE: " << yytext << std::endl;
"&&"                                                        std::cout << "BOOL_AND: " << yytext << std::endl;
"||"                                                        std::cout << "BOOL_OR: " << yytext << std::endl;
[ \t]+                                                      ;
[\r]?\n                                                     ++line_count;
#.*                                                         ;
.                                                           std::cout << "Unknown token on line " << ++line_count << ": " << yytext << std::endl; exit(1);
%%

int main
  (int argc, char* argv[]) 
{
  if (argc != 2)
  {
    std::cout<< "FORMATERROR: " << argv[0] << " [source filename] " <<\
    "\nprograme halted...\n";
    exit(1);
  }
  yyin = fopen(argv[1], "r");
  if (yyin == NULL)
  {
    std::cout<< "Error: input file not found!\nprograme halted...\n";
    fclose(yyin);
    exit(2);
  }
  mytokens = yylex();
  fclose(yyin); 
  std::cout << "Line Count: "<< line_count <<std::endl;
  return 0; 
}
