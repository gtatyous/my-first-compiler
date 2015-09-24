%{
#include <iostream>
#include <algorithm>
#include <fstream>
#include "SymbolTable.h"


extern int yylex();
extern FILE* yyin; 
//extern char* yytext;
int line_count =0; 
SymbolTable symbol_table;

void yyerror
  (char* err_string)
{
  std::cout << "ERROR (line " << ++line_count << "): syntax error" << std::endl;
  exit(1);
}

void check_redecl_error 
  (string type, string name)
{ 
  if (symbol_table.is_declared(name))
  {
    std::cout << "ERROR(line " << ++line_count << \
    "): redeclaration of variable '" << name << "'" << std::endl;
    exit(1);
  }
  else
  {
    symbol_table.insert(name);
    symbol_table.search(name)->type = type;
    symbol_table.search(name)->line = line_count;
    symbol_table.search(name)->init = false;
  }
}

void check_var
  (string name)
{
  if (!symbol_table.is_declared(name))
  {
    std::cout << "ERROR(line " << ++line_count \
    << "): unknown variable '" << name << "'" \
    << std::endl;
    exit(1);
  }
}

%}

%union 
{
  char* lexeme;
}

%token <lexeme> TYPE
%token COMMAND_PRINT
%token COMMAND_RANDOM
%token <lexeme> ID
%token VAL_LITERAL
%token CHAR_LITERAL
%token MULTI_CHAR
%token NON_TERM_CHAR
%token STRING_LITERAL
%token NON_TERM_STRING
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
%token NEW_LINE
%token UNKNOWN

%left ','
%left ASSIGN_ADD ASSIGN_SUB ASSIGN_MULT ASSIGN_DIV
%left BOOL_OR
%left BOOL_AND
%left COMP_EQU COMP_NEQU
%left COMP_LESS COMP_LTE COMP_GTR COMP_GTE 
%left '-' '+'
%left '*' '/'
%nonassoc UMINUS

%%

line: line line
    | NEW_LINE {++line_count;}
    | err
    | statement
    | {std::cout << "epsilon" << std::endl;}
    ;

err: MULTI_CHAR { /* report multi char error*/
                  std::cout << "ERROR(line " << \
                  ++line_count << "): syntax error" \
                  << std::endl;
                  exit(1);
                }
   | NON_TERM_CHAR { /* report non term char error*/
                     std::cout << "ERROR(line " << \
                     ++line_count << "): syntax error" \
                     << std::endl;
                     exit(1);
                   }
   | NON_TERM_STRING { /* report not term str error*/
                       std::cout << "ERROR(line " << \
                       ++line_count << "): syntax error" \
                       << std::endl;
                       exit(1);
                     }
   | UNKNOWN { /* report unknown char error*/
               std::cout << "ERROR(line " << \
               ++line_count << "): syntax error" \
               << std::endl;
               exit(1);
             }
   ;

statement: something ';'
         | ';'
         ;

something: decl 
         | mexpr  
         | cmd 
         ;

decl: TYPE ID {check_redecl_error($1, $2);}
    | TYPE ID '=' expr {check_redecl_error($1, $2);}
    ;

expr: expr opr1 expr
     | '(' expr ')'
     | '-' expr
     | term
     ;

opr1: '+'
    | '-'
    | '*'
    | '/'
    | COMP_EQU
    | COMP_NEQU
    | COMP_LESS
    | COMP_LTE
    | COMP_GTR
    | COMP_GTE
    ;

term: VAL_LITERAL
    | ID {check_var($1);}
    | COMMAND_RANDOM '(' expr ')'
    ;

mexpr: ID opr2 expr {check_var($1);}
    ;

opr2: '='
    | ASSIGN_ADD
    | ASSIGN_SUB
    | ASSIGN_MULT
    | ASSIGN_DIV
    ;

cmd: COMMAND_PRINT '(' list ')' 
   ;

list: expr
    | list ',' list
    |
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
  //std::cout << "#rules: " << YYNRULES << std::endl;
  yyparse();

  std::cout<< "Parse Successful!" << std::endl;
  //fclose(yyin);
  return 0;
}




