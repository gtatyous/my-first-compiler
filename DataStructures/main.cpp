#include "SyntaxTree.h"


int main ()
{

  AST* id = new ID_NODE("may_var");
  AST* val = new VAL_NODE(5.5);
  AST* opr = new OPR_NODE('+', id, val);
  opr->process();
  return 0;



}
