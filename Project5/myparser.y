%{

//#include <cstdlib> 
#include "../DataStructures/SyntaxTree.h"
#include "../DataStructures/SymbolTable.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream> 
#include <vector>
#include <typeinfo>

extern int yylex();
extern FILE* yyin; 
extern int line_count; 

std::ofstream outfile; //move this to main
std::stringstream TubeIC_out;
std::vector<SymbolTable*> my_stack;
SymbolTable* symbol_table;
int scope =0;
int loop_id; 
int while_counter = 0;

void yyerror
  (char* err_string)
{
  std::cout << "ERROR(line " << ++line_count << "): syntax error" << std::endl;
  exit(1);
}

void add_var
  (string type, string name)
{ 
  if (symbol_table->is_declared(name))  //only check in local scope
  {
    std::cout << "ERROR(line " << ++line_count << \
    "): redeclaration of variable '" << name << "'" << std::endl;
    exit(1);
  }
  else
  {
    symbol_table->insert(name, type, line_count, scope);
  }
}

var* check_var
  (string name)
{
  for (int i = my_stack.size()-1; i>=0; i--)
  {
    if (my_stack[i]->is_declared(name))
    {
      return  my_stack[i]->search(name);
    }
  }
  std::cout << "ERROR(line " << ++line_count \
  << "): unknown variable '" << name << "'" \
  << std::endl;
  exit(1);
}


%}

%union 
{
  char* lexeme;
  float value;
  AST* node;
}

%token IF
%token ELSE
%token WHILE
%token BREAK
%token CONTINUE
%token ARRAY
%token <lexeme> TYPE
%token COMMAND_PRINT
%token COMMAND_RANDOM
%token <lexeme> ID
%token <value> VAL_LITERAL
%token <lexeme>CHAR_LITERAL
%token <lexeme>STRING_LITERAL
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
%token BOOL_NOT
%token TERNARY
%token COLON
%token OPEN_BRACE
%token CLOSE_BRACE

%nonassoc IFX
%nonassoc ELSE

%left ','
%left ASSIGN_ADD ASSIGN_SUB ASSIGN_MULT ASSIGN_DIV
%right '='
%left BOOL_OR
%left BOOL_AND
%nonassoc COMP_EQU COMP_NEQU COMP_LESS COMP_LTE COMP_GTR COMP_GTE 
%left '-' '+'
%left '*' '/'
%nonassoc UMINUS BOOL_NOT


%type <node> statement_list
%type <node> statement
%type <node> decl
%type <node> expr 
%type <node> cmd
%type <node> list
%type <node> block

%%

program: statement_list {my_stack.pop_back(); 
                         $1->process();};

statement_list: { //global scope is = 0 (don't change it)
                  SymbolTable* new_scope = new SymbolTable();
                  my_stack.push_back(new_scope);
                  symbol_table = my_stack.back();
                  $$ = new AST_ROOT(new_scope);
                }
              | statement_list statement {$1->AddChild($2); $$ = $1;} 
              ;

statement: decl     ';' {$$ = $1;}
         | expr     ';' {$$ = $1;}
         | cmd      ';' {$$ = $1;}
         | BREAK    ';' {$$ = new BREAK_NODE();}
         | CONTINUE ';' {$$ = new CONTINUE_NODE();}
         |          ';' {$$ = new EMPTY_NODE();}
         | block        {$$ = $1;}
         | IF '(' expr ')' statement  %prec IFX {$$ = new IF_NODE($3, $5, NULL);}
         | IF '(' expr ')' statement ELSE statement {$$ = new IF_NODE($3, $5, $7);}
         | WHILE {while_counter++;} '(' expr ')' statement {$$ = new WHILE_NODE($4, $6);
                                                            while_counter--;}  
         ;

block: OPEN_BRACE {scope++;} statement_list CLOSE_BRACE {scope--;
                                           my_stack.pop_back();
                                           symbol_table = my_stack.back();
                                           $$ = $3;     };

decl: TYPE ID { add_var($1, $2); 
                AST* id = new ID_NODE($1, $2); 
                $$ = new DECL_NODE($1);
                $$->AddChild(id);
               }
    | TYPE ID {add_var($1, $2);} '=' expr { AST* id  = new ID_NODE($1, $2); 
                                            AST* opr_node = new OPR_NODE("=", id, $5);
                                            $$ = new DECL_NODE($1); 
                                            $$->AddChild(opr_node);
                                          }
    | ARRAY '(' TYPE ')' ID { 
                            std::string array_t = std::string("array") + "(" + $3 + ")";
                            add_var(array_t, $5); 
                            AST* id = new ID_NODE(array_t, $5); //ID for array
                            $$ = new DECL_NODE(array_t);
                            $$->AddChild(id);
                            }
    | ARRAY '(' TYPE ')' ID  '=' expr { 
                            std::string array_t = std::string("array") + "(" + $3 + ")";
                            add_var(array_t, $5); 
                            AST* id = new ID_NODE(array_t, $5); //ID for array
                            AST* opr_node = new OPR_NODE("=", id, $7);
                            $$ = new DECL_NODE(array_t); 
                            $$->AddChild(opr_node);
                                      }
    | decl ',' ID {add_var($1->GetType(), $3);}  { std::string t = $1->GetType();
                                                   AST* id  = new ID_NODE(t, $3); 
                                                   $1->AddChild(id); 
                                                   $$ = $1;
                                                 }
    | decl ',' ID {add_var($1->GetType(), $3);} '=' expr   { 
                                                   std::string t = $1->GetType();
                                                   AST* id  = new ID_NODE(t, $3); 
                                                   AST* opr_node = new OPR_NODE("=", id, $6);
                                                   $1->AddChild(opr_node); 
                                                   $$ = $1;
                                                           };

expr: ID {check_var($1);}    '='     expr {
                                    std::string t = check_var($1)->type ;
                                    AST* id = new ID_NODE(t, $1); 
                                    $$ = new OPR_NODE("=", id, $4);
                                          }
    | ID {check_var($1);} ASSIGN_ADD expr { 
                                    std::string t = check_var($1)->type ;
                                    AST* id = new ID_NODE(t, $1); 
                                    $$ = new OPR_NODE("+=", id, $4);
                                   }
    | ID {check_var($1);} ASSIGN_SUB expr {
                                    std::string t = check_var($1)->type ;
                                    AST* id = new ID_NODE(t, $1);
                                    $$ = new OPR_NODE("-=", id, $4);
                                   }
    | ID {check_var($1);} ASSIGN_MULT expr {
                                    std::string t = check_var($1)->type ;
                                    AST* id = new ID_NODE(t, $1);
                                    $$ = new OPR_NODE("*=", id, $4);
                                   }
    | ID {check_var($1);} ASSIGN_DIV expr {
                                    std::string t = check_var($1)->type ;
                                    AST* id = new ID_NODE(t, $1);
                                    $$ = new OPR_NODE("/=", id, $4);
                                   }

    | expr    '+'    expr   { $$  = new OPR_NODE ("+",   $1 , $3);}
    | expr    '-'    expr   { $$  = new OPR_NODE ("-",   $1 , $3);}
    | expr    '*'    expr   { $$  = new OPR_NODE ("*",   $1 , $3);}
    | expr    '/'    expr   { $$  = new OPR_NODE ("/",   $1 , $3);}
    | expr COMP_EQU  expr   { $$ = new OPR_NODE  ("==",  $1 , $3);} 
    | expr COMP_NEQU expr   { $$ = new OPR_NODE  ("!=",  $1 , $3);}
    | expr COMP_LESS expr   { $$ = new OPR_NODE  ("<" ,  $1 , $3);}
    | expr COMP_LTE  expr   { $$ = new OPR_NODE  ("<=",  $1 , $3);}
    | expr COMP_GTR  expr   { $$ = new OPR_NODE  (">" ,  $1 , $3);}
    | expr COMP_GTE  expr   { $$ = new OPR_NODE  (">=",  $1 , $3);}
    | expr BOOL_AND  expr   { $$ = new BOOL_NODE ("&&",  $1 , $3);}
    | expr BOOL_OR   expr   { $$ = new BOOL_NODE ("||",  $1 , $3);}
    | BOOL_NOT expr         { $$ = new BOOL_NODE ("!" , NULL, $2);}
    | expr TERNARY expr COLON expr  {$$ = new TERNARY_OPR_NODE($1, $3, $5);}
    | '(' expr ')' {$$ = $2;}
    | '-' expr {$$ = new UMINUS_NODE($2);}
    | VAL_LITERAL {$$ = new VAL_NODE($1);}
    | CHAR_LITERAL {$$ = new CHAR_NODE($1);}
    | STRING_LITERAL {$$ = new STRING_NODE($1);}
    | ID {std::string t = check_var($1)->type ; $$ = new ID_NODE(t, $1);}
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
    //fclose(yyin);
    exit(2);
  }
  yyparse();
  
  outfile.open(argv[2]);
  outfile << TubeIC_out.str();
  outfile.close();
  /*for (std::size_t i=0; i<my_stack; i++)
  {
    delete my_stack[i];
  }*/
  std::cout<< "Parse Successful!" << std::endl;
  return 0;
}




