%{
#include "test1.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
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
\'[^'\n]?\'                                                    return CHAR_LITERAL;
\"[^"\n]*\"                                                    return STRING_LITERAL;
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
[ \t]+                                                    return WHITESPACE;
[\r]?\n                                                   ++line_count; return WHITESPACE;
#.*                                                       return COMMENT;
.                                                         ++line_count; return UNKNOWN;
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
  std::stringstream out;
  mytokens = yylex();
  while (mytokens)
  {
    switch (mytokens)
    {
      case TYPE:
        out << "TYPE: " << yytext << "\n";
        break;
      case COMMAND_PRINT:
        out << "COMMAND_PRINT: " << yytext << "\n";
        break;
      case COMMAND_RANDOM:
        out << "COMMAND_RANDOM: " << yytext << "\n";
        break;
      case ID:
        out << "ID: " << yytext << "\n";
        break;
      case VAL_LITERAL:
        out << "VALUE_LITERAL: " << yytext << "\n";
        break;
     case CHAR_LITERAL:
        out << "CHAR_LITERAL: " << yytext << "\n";
        break;
      case STRING_LITERAL:
        out << "STRING_LITERAL: " << yytext << "\n";
        break;
      case ASCII_CHAR:
        out << "ASCII_CHAR: " << yytext << "\n";
        break;
      case ASSIGN_ADD:
        out << "ASSIGN_ADD: " << yytext << "\n";
        break;
      case ASSIGN_SUB:
        out << "ASSIGN_SUB: " << yytext << "\n";
        break;
      case ASSIGN_MULT: 
        out << "ASSIGN_MULT: " << yytext << "\n";
        break;
      case ASSIGN_DIV:
        out << "ASSIGN_DIV: " << yytext << "\n";
        break;
      case COMP_EQU:
        out << "COMP_EQU: " << yytext << "\n";
        break;
      case COMP_NEQU:
        out << "COMP_NEQU: " << yytext << "\n";
        break;
      case COMP_LESS:
        out << "COMP_LESS: " << yytext << "\n";
        break;
      case COMP_LTE:
        out << "COMP_LTE: " << yytext << "\n";
        break;
      case COMP_GTR:
        out << "COMP_GTR: " << yytext << "\n";
        break;
      case COMP_GTE:
        out << "COMP_GTE: " << yytext << "\n";
        break;
      case BOOL_AND:
        out << "BOOL_AND: " << yytext << "\n";
        break;
      case BOOL_OR:
        out << "BOOL_OR: " << yytext << "\n";
        break;
      case UNKNOWN:
        out << "Unknow token on line " << line_count << ": " << yytext << "\n";
        fclose(yyin); std::cout<< out.str();
        exit(3);
        break;
    }
    mytokens = yylex();
  }
  out<< "Line Count: "<< line_count <<std::endl;
  std::cout<< out.str();
  fclose(yyin);
  return 0; 
}
