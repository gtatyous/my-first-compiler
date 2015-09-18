%{
#include <iostream>
#include <algorithm>
#include <fstream>
void yyerror(char* err_string)
{
  std::cout << "Input failed to match" << std::endl;
}
extern int yylex();
extern FILE* yyin; 
int line_count;
%}


%token TYPE
%token COMMAND_PRINT
%token COMMAND_RANDOM
%token ID
%token VAL_LITERAL
%token CHAR_LITERAL
%token MULTI_CHAR
%token NON_TERM_CHAR
%token STRING_LITERAL
%token NON_TERM_STRING
%token ASCII_CHAR
%token ASSIGN_ADD
%token ASSIGN_SUB
%token ASSIGN_MULT
%token ASSIGN_DIV
%token COMP_EQU
%token COMP_NEQU
%token COMP_LESS
%token COMP_LTE
%token COMP_GTR
%token COMP_GTE
%token BOOL_AND
%token BOOL_OR
%token WHITESPACE
%token NEW_LINE
%token COMMENT
%token MULTI_LINE_COMMENT
%token UNKNOWN


%%
line: line statment  
    | /* nothing */  {std::cout<< "nothing"<< std::endl;}
    ;
statment: decl ';'  {std::cout << "declaration" << std::endl;}
        | expr ';'  {std::cout << "experation " << std::endl;}
        | cmd  ';'  {std::cout << "command yo " << std::endl;}
        ;
decl: TYPE ID 
    | TYPE ID OPR expr
    ;
expr: VAL_LITERAL OPR VAL_LITERAL
    ;
cmd: COMMAND_PRINT '(' OUT ')'
   ;
OPR: ASCII_CHAR
   ;
OUT: VAL_LITERAL 
   | ID
   ;
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
  yyparse();

  std::cout<< "successfully compiled!";
  fclose(yyin);
  return 0;
}




