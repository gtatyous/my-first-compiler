%{
#include <iostream>
#include <algorithm>
#include <fstream>
#include "../DataStructures/SymbolTable.h"
#include "../DataStructures/SyntaxTree.h"

extern int yylex();
extern FILE* yyin; 
//extern char* yytext;
extern int line_count; 

SymbolTable symbol_table;

void yyerror
  (char* err_string)
{
  std::cout << "ERROR(line " << ++line_count << "): syntax error" << std::endl;
  exit(1);
}

void add_var
  (string name)
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
    symbol_table.search(name)->line = line_count;
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
%token STRING_LITERAL
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

program: {$$ = new SyntexTree();}
       | program statement {$1->AddChild($2);}
       ;

statement: decl ';' {$$=$1;}
         | expr ';' {$$=$1;}
         | cmd  ';' {$$=$1;}
         |      ';'
         ;

decl: TYPE ID {add_var($2);}
    | TYPE ID {add_var($2);} '=' expr
    | decl ',' multidecl
    ;

multidecl: ID {add_var($1);}
         | ID {add_var($1);} '=' expr
         ;

expr: ID {check_var($1);} opr expr
    | expr '+' expr
    | expr '-' expr
    | expr '*' expr
    | expr '/' expr
    | expr COMP_EQU expr
    | expr COMP_NEQU expr
    | expr COMP_LESS expr
    | expr COMP_LTE expr
    | expr COMP_GTR expr
    | expr COMP_GTE expr
    | expr BOOL_AND expr
    | expr BOOL_OR expr
    | '(' expr ')'
    | '-' expr
    | VAL_LITERAL {$$ = new VAL_NODE($1);}
    | ID {check_var($1); $$ = new ID_NODE($1);}
    | COMMAND_RANDOM '(' expr ')'
    ;

opr : '='
    | ASSIGN_ADD
    | ASSIGN_SUB
    | ASSIGN_MULT
    | ASSIGN_DIV
    ;

cmd: COMMAND_PRINT '(' list ')' 
   ;

list: expr
    | list ',' list
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




