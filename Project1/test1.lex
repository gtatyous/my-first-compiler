%{
#include "test1.hpp"
#include <iostream>
#include <fstream>
int line_count=0, mytokens = 0;
%}

%option noyywrap
%option yylineno

%%
val|char|string                                           return TYPE;
print                                                     return COMMAND_PRINT;
random                                                    return COMMAND_RANDOM;
[a-zA-Z_][a-zA-Z_0-9]*                                    return ID;
[+\-]?[0-9]+(\.[0-9]+)?([eE][+\-]?[0-9]+)?                return VAL_LITERAL;
\'.?\'                                                    return CHAR_LITERAL;
\".*\"                                                    return STRING_LITERAL;
[+\-*/()=,{}\[\]\.;]                                      return ASCII_CHAR;
\+=                                                       return ASSIGN_ADD;
-=                                                        return ASSIGN_SUB;
\*=                                                       return ASSIGN_MULT;
"/="                                                      return ASSIGN_DIV;
"=="                                                      return COMP_EQU;
"!="                                                      return COMP_NEQU;
"<"                                                       return COMP_LESS; 
"<="                                                      return COMP_LTE;
">"                                                       return COMP_GTR;
">="                                                      return COMP_GTE;
"&&"                                                      return BOOL_AND;
"||"                                                      return BOOL_OR;
[ \t]+                                                    ;
[\r]?\n                                                   ++line_count;
#.*                                                       ;
.                                                         return UNKNOWN;
%%

int main(int argc, char* argv[]) 
{
  /*FlexLexer* lexer = new yyFlexLexer();
  while (lexer->yylex())
  {
    std::cout << "yytext: " << yytext << std::endl;
  }*/

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
  std::ofstream output_file;
  output_file.open("out.tube");
  mytokens = yylex();
  while (mytokens)
  {
    switch (mytokens)
    {
      case TYPE:
        output_file << "TYPE: " << yytext << "\n";
        break;
      case COMMAND_PRINT:
        output_file << "COMMAND_PRINT: " << yytext << "\n";
        break;
      case COMMAND_RANDOM:
        output_file << "COMMAND_RANDOM: " << yytext << "\n";
        break;
      case ID:
        output_file << "ID: " << yytext << "\n";
        break;
      case VAL_LITERAL:
        output_file << "VALUE_LITERAL: " << yytext << "\n";
        break;
     case CHAR_LITERAL:
        output_file << "CHAR_LITERAL;: " << yytext << "\n";
        break;
      case STRING_LITERAL:
        output_file << "STRING_LITERAL: " << yytext << "\n";
        break;
      case ASCII_CHAR:
        output_file << "ASCII_CHAR: " << yytext << "\n";
        break;
      case ASSIGN_ADD:
        output_file << "ASSIGN_ADD: " << yytext << "\n";
        break;
      case ASSIGN_SUB:
        output_file << "ASSIGN_SUB: " << yytext << "\n";
        break;
      case ASSIGN_MULT: 
        output_file << "ASSIGN_MULT: " << yytext << "\n";
        break;
      case ASSIGN_DIV:
        output_file << "ASSIGN_DIV: " << yytext << "\n";
        break;
      case COMP_EQU:
        output_file << "COMP_EQU: " << yytext << "\n";
        break;
      case COMP_NEQU:
        output_file << "COMP_NEQU: " << yytext << "\n";
        break;
      case COMP_LESS:
        output_file << "COMP_LESS: " << yytext << "\n";
        break;
      case COMP_LTE:
        output_file << "COMP_LTE: " << yytext << "\n";
        break;
      case COMP_GTR:
        output_file << "COMP_GTR: " << yytext << "\n";
        break;
      case COMP_GTE:
        output_file << "COMP_GTE: " << yytext << "\n";
        break;
      case BOOL_AND:
        output_file << "BOOL_AND: " << yytext << "\n";
        break;
      case BOOL_OR:
        output_file << "BOOL_OR: " << yytext << "\n";
        break;
      case UNKNOWN:
        output_file << "Unknow token on line " << ++line_count << ": " << yytext << "\n";
        fclose(yyin); output_file.close();
        exit(3);
        break;
      default:
        output_file << "default: " << yytext << "\n";
        break;
    }
    mytokens = yylex();
  }
  output_file<< "Line Count: "<< line_count <<std::endl;
  output_file.close();
  fclose(yyin);
  return 0; 
}
