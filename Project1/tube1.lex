%{
#include "tokens.hpp"
#include <iostream>
#include <algorithm>
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
([0-9]+(\.[0-9]+)?|(\.[0-9]+)){1}([eE][+\-]?[0-9]+)?      return VAL_LITERAL;
\'[^'\n]?\'                                               return CHAR_LITERAL;
\'[^'\n]{2,}\'                                            return MULTI_CHAR;
\'                                                        return NON_TERM_CHAR;
\"[^"\n]*\"                                               return STRING_LITERAL;
\"                                                        return NON_TERM_STRING;
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
"/*"(.|[\r\n])*"*/"                                      return MULTI_LINE_COMMENT;
.                                                         return UNKNOWN;
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
        out << "VAL_LITERAL: " << yytext << "\n";
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
      case MULTI_LINE_COMMENT:
      {
        std::string com(yytext); //convert char* to string
        line_count += std::count(com.begin(), com.end(), '\n');
        break;
      }
      case UNKNOWN:
        out << "Unknown token on line " << ++line_count << ": " << yytext << "\n";
        mytokens = EXIT;
        break;
      case NON_TERM_STRING:
        out << "ERROR(line " << ++line_count << "): Unterminated string\n"; 
        mytokens=EXIT;
        break;
      case NON_TERM_CHAR:
        out << "ERROR(line " << ++line_count << "): Uterminated char\n"; 
        mytokens=EXIT;
        break;
      case MULTI_CHAR:
        out << "ERRRO(line " << ++line_count << "): Mutli char literal\n"; 
        mytokens=EXIT;
        break;
    }
    if (mytokens == EXIT)
    {
      fclose(yyin); 
      std::cout<< out.str(); 
      exit(3);
    }
    mytokens = yylex();
  }
  out<< "Line Count: "<< line_count <<std::endl;
  std::cout<< out.str();
  fclose(yyin);
  return 0; 
}
