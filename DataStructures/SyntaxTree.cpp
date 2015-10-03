#include "SyntaxTree.h"
#include <iostream>

void AST_ROOT::AddChild 
  (AST* child)
{
  _children.push_back(child);
}
void ID_NODE::print
  (void) 
{

}

int ID_NODE::process
  (void)
{
  int out_id = symbol_table.search(_name)->id;
  if (out_id == -1)
  {
    int out_id = GetID();
    symbol_table.search(_name)->id = out_id;
  }
  return out_id;
}

void VAL_NODE::print
  (void) 
{

}

int VAL_NODE::process
  (void)
{
  int out_id = GetID();
  std::cout << "val_copy " << _val << " s" << out_id << std::endl;
  return out_id;
}

OPR_NODE::OPR_NODE
  (std::string opr, AST* LHS, AST* RHS)
  : _opr (opr)
{
  _children.push_back(LHS);
  _children.push_back(RHS);
}

void OPR_NODE::print
  (void) 
{

}

int OPR_NODE::process
  (void)
{
  int lhs = _children[0]->process();
  int rhs = _children[1]->process();

  if      (_opr == "+")   {std::cout << "add s";}
  else if (_opr == "-")   {std::cout << "sub s";}
  else if (_opr == "*")   {std::cout << "mult s";}
  else if (_opr == "/")   {std::cout << "div s";}
  else if (_opr == "=")   {std::cout << "val_copy s" << rhs << " s" << lhs << std::endl; return lhs;}
  else if (_opr == "==")  {std::cout << "test_equ s";}
  else if (_opr == "!=")  {std::cout << "test_nequ s";} 
  else if (_opr == "<")   {std::cout << "test_less s";}
  else if (_opr == "<=")  {std::cout << "test_lte s";}
  else if (_opr == ">")   {std::cout << "test_gtr s";}
  else if (_opr == ">=")  {std::cout << "test_gte s";}
  else if (_opr == "&&")  {std::cout << "test_and s";}
  else if (_opr == "||")  {std::cout << "test_or s";}
  else {std::cout << "Internal Compiler ERROR!!" << std::endl;}

  int out_id = GetID();
  std::cout << lhs << " s"<<rhs << " s"<<out_id << std::endl;
  return out_id;
}

void CMD_NODE::print
  (void)
{
}

int CMD_NODE::process
  (void) 
{

}
