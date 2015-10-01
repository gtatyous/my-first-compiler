#include "SyntaxTree.h"

void ID_NODE::print() 
{

}

int ID_NODE::process()
{
  int out_id = GetID();
  return out_id;
}


void VAL_NODE::print() 
{

}

int VAL_NODE::process()
{
  int out_id = GetID();
  std::cout << "val_copy " << _val << " s" << out_id << std::endl;
  return out_id;
}

OPR_NODE::OPR_NODE
  (char opr, AST* LHS, AST* RHS)
  : _opr (opr)
{
  _children.push_back(LHS);
  _children.push_back(RHS);
}

void OPR_NODE::print() 
{

}

int OPR_NODE::process()
{
  int lhs = _children[0]->process();
  int rhs = _children[1]->process();
  int out_id = GetID();

  switch(_opr)
  {
    case '+': std::cout << "add s"; break;
    case '-': std::cout << "sub s"; break;
    case '*': std::cout << "mult s"; break;
    case '/': std::cout << "div s"; break;
    default:
      std::cerr << "Internal Compiler ERROR!!" << std::endl;
  }
  std::cout << lhs << " s"<<rhs << " s"<<out_id << std::endl;
  return out_id;
}
