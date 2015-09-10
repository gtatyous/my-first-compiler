%{
#include "myscanner.h"
%}

%%
"val"                        return TYPE
"char"                       return TYPE
"string"                     return TYPE
"print"                      return COMMAND_PRINT
"random"                     return COMMAND_RANDOM
[a-zA-Z_][a-zA-Z0-9_]*       return ID
[0-9]+[\.0-9+]*              return VAL_LITERAL
.                            return UNKNOWN
%%


