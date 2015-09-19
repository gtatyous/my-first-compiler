%{
#include "myparser.tab.hh"
#include <iostream>
extern int yyparse();
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
[ \t]+                                                    ;
[\r]?\n                                                   return NEW_LINE;
#.*                                                       ;
"/*"(.|[\r\n])*"*/"                                       ;
.                                                         return UNKNOWN;
