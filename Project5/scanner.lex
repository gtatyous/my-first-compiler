%{
#include "../DataStructures/SyntaxTree.h"
#include "tube5.tab.hh"
#include <iostream>
#include <string.h>
#include <algorithm>
#include <vector>

int line_count = 0; 

%}

%option noyywrap
%option yylineno

%%
if                       {return IF;}
else                     {return ELSE;}
while                    {return WHILE;}
break                    {return BREAK;}
continue                 {return CONTINUE;}
array                    {return ARRAY;}
size                     {return SIZE;}
resize                   {return RESIZE;}
val|char|string          {yylval.lexeme = strdup(yytext); return TYPE;}
print                    return COMMAND_PRINT;
random                   return COMMAND_RANDOM;
[a-zA-Z_][a-zA-Z_0-9]*   {yylval.lexeme = strdup(yytext); return ID;}
([0-9]+(\.[0-9]+)?|(\.[0-9]+)){1}([eE][+\-]?[0-9]+)? { yylval.value = atof(yytext);
                                                       return VAL_LITERAL;}
\'[^'\\]?\'|\'\\n\'|\'\\t\'|\'\\\'\'|\'\\\\\'  {yylval.lexeme = strdup(yytext);
                                                return CHAR_LITERAL;}
\'[^'\n]{2,}\'           { /* report multi char error*/
                  	       std::cout << "ERROR(line " << \
                           ++line_count << "): syntax error" \
                           << std::endl;
                           exit(1);
                         }
\'                       { /* report non term char error*/
                           std::cout << "ERROR(line " << \
                           ++line_count << "): syntax error" \
                           << std::endl;
                           exit(1);
                         }
\"([^"\\]|\\n|\\t|\\\"|\\\\)*\"                 { yylval.lexeme = strdup(yytext);
                                                  return STRING_LITERAL;}
\"                       { /* report not term str error*/
                           std::cout << "ERROR(line " << \
                           ++line_count << "): syntax error" \
                           << std::endl;
                           exit(1);
                         }
[+\-*/()=,\[\]\.;]       return yytext[0];
\+=                      return ASSIGN_ADD;
-=                       return ASSIGN_SUB;
\*=                      return ASSIGN_MULT;
"/="                     return ASSIGN_DIV;
"=="                     return COMP_EQU;
"!="                     return COMP_NEQU;
"<"                      return COMP_LESS; 
"<="                     return COMP_LTE;
">"                      return COMP_GTR;
">="                     return COMP_GTE;
"&&"                     return BOOL_AND;
"||"                     return BOOL_OR;
"!"                      return BOOL_NOT;
[ \t\r]+                 { /*ignore whitespaces*/}
\n                       {++line_count;}
#.*                      { /*ignore comments*/}
"/*"(.|[\r\n])*"*/"      {std::string com(yytext);
                          line_count += std::count(com.begin(), com.end(), '\n');}
"?"                      return TERNARY;
":"                      return COLON;
"{"                      return OPEN_BRACE;
"}"                      return CLOSE_BRACE;            
.                        { /* report unknown char error*/
               		         std::cout << "ERROR(line " << \
                           ++line_count << "): unknown token " << yytext[0] \
                           << std::endl;
                           exit(1);}
%%
