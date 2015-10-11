%{

#include "../DataStructures/SyntaxTree.h"
#include "../DataStructures/SymbolTable.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream> 

extern int yylex();
extern FILE* yyin; 
extern int line_count; 

std::ofstream outfile;
std::stringstream TubeIC_out;
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
  float value;
  AST* node;
}

%token <lexeme> TYPE
%token COMMAND_PRINT
%token COMMAND_RANDOM
%token <lexeme> ID
%token <value> VAL_LITERAL
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
%token COP_GTE
%token BOOL_AND
%token BOOL_OR
%token TERNARY
%token COLON

%left ','
%left ASSIGN_ADD ASSIGN_SUB ASSIGN_MULT ASSIGN_DIV
%right '='
%left BOOL_OR
%left BOOL_AND
%nonassoc COMP_EQU COMP_NEQU COMP_LESS COMP_LTE COMP_GTR COMP_GTE 
%left '-' '+'
%left '*' '/'
%nonassoc UMINUS

%type <node> program
%type <node> statement
%type <node> decl
%type <node> expr 
%type <node> cmd
%type <node> list

%%

program: { $$ = new AST_ROOT(); }
       | program statement { $1->AddChild($2);}
       ;

statement: decl ';' {$1->process(); $$ = $1;}
         | expr ';' {$1->process(); $$ = $1;}
         | cmd  ';' {$1->process(); $$ = $1;}
         |      ';' {/*do nothing*/ $$ = new EMPTY_NODE();}
         ;

decl: TYPE ID { add_var($2); 
                AST* id = new ID_NODE($2); 
                $$ = new DECL_NODE();
                $$->AddChild(id);
               }
    | TYPE ID {add_var($2);} '=' expr { AST* id  = new ID_NODE($2); 
                                        AST* opr_node = new OPR_NODE("=", id, $5);
                                        $$ = new DECL_NODE(); 
                                        $$->AddChild(opr_node);
                                      }
    | decl ',' ID {add_var($3);}  { AST* id  = new ID_NODE($3); 
                                    $1->AddChild(id); 
                                    $$ = $1;
                                  }
    | decl ',' ID {add_var($3);} '=' expr   { AST* id  = new ID_NODE($3); 
                                              AST* opr_node = new OPR_NODE("=", id, $6);
                                              $1->AddChild(opr_node); 
                                              $$ = $1;
                                            }
    ;

expr: ID {check_var($1);} '=' expr {
                                    AST* id = new ID_NODE($1); 
                                    $$ = new OPR_NODE("=", id, $4);
                                   }
    | ID {check_var($1);} ASSIGN_ADD expr {
                                    AST* id = new ID_NODE($1); 
                                    $$ = new OPR_NODE("+=", id, $4);
                                   }
    | ID {check_var($1);} ASSIGN_SUB expr {
                                    AST* id = new ID_NODE($1); 
                                    $$ = new OPR_NODE("-=", id, $4);
                                   }
   | ID {check_var($1);} ASSIGN_MULT expr {
                                    AST* id = new ID_NODE($1); 
                                    $$ = new OPR_NODE("*=", id, $4);
                                   }
    | ID {check_var($1);} ASSIGN_DIV expr {
                                    AST* id = new ID_NODE($1); 
                                    $$ = new OPR_NODE("/=", id, $4);
                                   }

    | expr    '+'    expr   {$$  = new OPR_NODE("+",  $1, $3);}
    | expr    '-'    expr   {$$  = new OPR_NODE("-",  $1, $3);}
    | expr    '*'    expr   {$$  = new OPR_NODE("*",  $1, $3);}
    | expr    '/'    expr   {$$  = new OPR_NODE("/",  $1, $3);}
    | expr COMP_EQU  expr   { $$ = new OPR_NODE("==", $1, $3);} 
    | expr COMP_NEQU expr   { $$ = new OPR_NODE("!=", $1, $3);}
    | expr COMP_LESS expr   { $$ = new OPR_NODE("<" , $1, $3);}
    | expr COMP_LTE  expr   { $$ = new OPR_NODE("<=", $1, $3);}
    | expr COMP_GTR  expr   { $$ = new OPR_NODE(">" , $1, $3);}
    | expr COMP_GTE  expr   { $$ = new OPR_NODE(">=", $1, $3);}
    | expr BOOL_AND  expr   { $$ = new BOOL_NODE("&&", $1, $3);}
    | expr BOOL_OR   expr   { $$ = new BOOL_NODE("||", $1, $3);}
    | expr TERNARY expr COLON expr  {$$ = new TERNARY_OPR_NODE($1, $3, $5);}
    | '(' expr ')' {$$ = $2;}
    | '-' expr {$$ = new UMINUS_NODE($2);}
    | VAL_LITERAL {$$ = new VAL_NODE($1);}
    | ID {check_var($1); $$ = new ID_NODE($1);}
    | COMMAND_RANDOM '(' expr ')'  {$$ = new RAND_CMD_NODE($3);}
    ;

cmd: COMMAND_PRINT '(' list ')'  {$$ = $3;} 
   ;

list: expr {$$ = new PRINT_CMD_NODE(); $$->AddChild($1);}
    | list ',' expr {$1->AddChild($3); $$ = $1;}
    ;

%%


int main
  (int argc, char* argv[])
{
  if (argc != 3)
  {
    std::cout<< "FORMATERROR: " << argv[0] << " [source filename] [output filename] " \
    << "\nprograme halted...\n";
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

  outfile.open(argv[2]);
  outfile << TubeIC_out.str();
  outfile.close();
  std::cout<< "Parse Successful!" << std::endl;
  return 0;
}




