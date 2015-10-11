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
    out_id = GetID();
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
  TubeIC_out << "val_copy " << _val << " s" << out_id << std::endl;
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

  if      (_opr == "+")   {TubeIC_out << "add s";}
  else if (_opr == "-")   {TubeIC_out << "sub s";}
  else if (_opr == "*")   {TubeIC_out << "mult s";}
  else if (_opr == "/")   {TubeIC_out << "div s";}
  else if (_opr == "=")   {TubeIC_out << "val_copy s" << rhs << " s" << lhs << std::endl; return lhs;}
  else if (_opr == "==")  {TubeIC_out << "test_equ s";}
  else if (_opr == "!=")  {TubeIC_out << "test_nequ s";} 
  else if (_opr == "<")   {TubeIC_out << "test_less s";}
  else if (_opr == "<=")  {TubeIC_out << "test_lte s";}
  else if (_opr == ">")   {TubeIC_out << "test_gtr s";}
  else if (_opr == ">=")  {TubeIC_out << "test_gte s";}
  else if (_opr == "&&")  {TubeIC_out << "test_and s";}
  else if (_opr == "||")  {TubeIC_out << "test_or s";}
  else {std::cout << "Internal Compiler ERROR!!" << std::endl;}

  int out_id = GetID();
  TubeIC_out << lhs << " s"<<rhs << " s"<<out_id << std::endl;
  return out_id;
}

void PRINT_NODE::print
  (void)
{
}

void PRINT_NODE::AddChild 
  (AST* child)
{
  _children.push_back(child);
}

int PRINT_NODE::process
  (void)
{
  
  for (int i=0; i<_children.size(); i++)
  {
    _outChars.push_back(_children[i]->process());
  }

  for (int j=0; j<_outChars.size(); j++)
  {
    TubeIC_out << "out_val s" << _outChars[j] << std::endl;
  }
  TubeIC_out << "out_char " << "'\\n'" << std::endl;
  return -1;
}

UMINUS_NODE::UMINUS_NODE
  (AST* expr)
{
  _children.push_back(expr);
}

int UMINUS_NODE::process
  (void)
{
  int expr = _children[0]->process();
  int out_id = GetID();
  TubeIC_out << "mult -1 s"<< expr <<" s"<< out_id << std::endl;
  return out_id;
}

void UMINUS_NODE::print
  (void)
{
  
}

