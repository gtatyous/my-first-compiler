#include "SyntaxTree.h"

///////////////////////////////////root
void AST_ROOT::AddChild 
  (AST* child)
{
  _children.push_back(child);
}

//////////////////////////////////ID
int ID_NODE::process
  (void)
{
  int out_id  = check_var(_name)->id;
  if (out_id == -1)
  {
    out_id = GetID();
    symbol_table->search(_name)->id = out_id;
  }
  return out_id;
}

/////////////////////////////////val literal
int VAL_NODE::process
  (void)
{
  int out_id = GetID();
  TubeIC_out << "val_copy " << _val << " s" << out_id << std::endl;
  return out_id;
}

/////////////////////////////////char literal
int CHAR_NODE::process
  (void)
{
  int out_id = GetID();
  TubeIC_out << "val_copy " << _char << " s" << out_id << std::endl;
  return out_id;
}

/////////////////////////////////string literal
ARRAY_OPR_NODE::ARRAY_OPR_NODE
  (std::string opr, std::string name, AST* LHS, AST* RHS, AST* index_node)
  : _opr (opr), _name(name)
{
  _children.push_back(LHS);
  _children.push_back(RHS);
  _children.push_back(index_node);

  std::string id_type  = check_var(name)->type;
  std::string lhs_type = LHS->GetType();
  std::string rhs_type = RHS->GetType();
  
  //std::cout << "id_type: " << id_type << std::endl;
  //std::cout << "lhs_type: " << lhs_type << std::endl;
  //std::cout << "rhs_type: " << rhs_type << std::endl;

  _type = id_type;
  //check if lhs type is == val. otherwise can't index
  //check if id type == array(char) and rhs == char then its good
}

int ARRAY_OPR_NODE::process
  (void)
{
  int ar_id  = check_var(_name)->id;
  int rhs_id = _children[1]->process();
  
  if(_opr == "*=")
  {
    int rhs2_id = _children[2]->process();
    TubeIC_out << "mult s" << rhs2_id << " s" << rhs_id << " s";
    rhs_id = GetID();
    TubeIC_out << rhs_id <<std::endl;
  }

  int index_id = _children[0]->process();
  TubeIC_out << "ar_set_idx a" << ar_id << " s" << index_id \
             << " s" << rhs_id << std::endl;
  return ar_id; //??
}

int INDEX_NODE::process
  (void)
{
  int expr_id = _children[0]->process();
  int ar_id   = check_var(_name)->id;
  int out_id  = GetID();
  TubeIC_out << "ar_get_idx a" << ar_id << " s" << expr_id \
             << " s" << out_id << std::endl;
  return out_id;
}
  
int ARRAY_CHAR_NODE::process
  (void)
{
  int out_id = GetID();
  TubeIC_out << "ar_set_siz a" << out_id << " " << _size << std::endl;
  int i = 1; 
  int index = 0;
  while(i<_str.length()-1)
  {
    TubeIC_out << "ar_set_idx a" << out_id << " " << index++  << " "   \
               << "'";
    if (_str[i] == '\\')
    {
      //std::cout << "i: " << i << "  str[i]: " << _str[i] << std::endl;
      //std::cout << "str[++i]: " << _str[++i] << std::endl;
      //std::cout << "now i: " << i << " str[i]: " << _str[i] <<std::endl <<std::endl;
      TubeIC_out << _str[i];
      i++;
      TubeIC_out << _str[i];
    }
    else 
    {
      TubeIC_out << _str[i];
    }
    TubeIC_out << "'" <<  std::endl;
    i++;
  }
  return out_id;
}

///////////////////////////////operators and bool
OPR_NODE::OPR_NODE
  (std::string opr, AST* LHS, AST* RHS)
  : _opr (opr)
{
  _children.push_back(LHS);
  _children.push_back(RHS);
  std::string lhs_type = LHS->GetType();
  std::string rhs_type = RHS->GetType();

  //if lhs or rhs == type of array(char) or array(val)
    //if _opr == "="
      //check if rhs is a type of string for  array(char)
      //check if rhs is a type of ?????? for  array(val)
    //else
      //ERROR(line #): cannot use type 'type' in mathematical expressions

  if ( (lhs_type == "char" or
        rhs_type == "char"  )     and
            (_opr == "+"   or
             _opr == "-"   or
             _opr == "/"   or
             _opr == "*"   or
             _opr == "+="  or
             _opr == "-="  or
             _opr == "/="  or
             _opr == "*="   )       )
  {
    std::cout << "ERROR(line "<< ++line_count << "): cannot use type '"  \
              << "char" << "' mathematical expressions" << std::endl;
    exit(1);
  }

  else if (lhs_type == "string" and rhs_type == "array(char)")
  {
    /* skip type checking */
  }

  else if (lhs_type != rhs_type)
  {
    if (  _opr == "+="       or
          _opr == "-="       or
          _opr == "/="       or
          _opr == "*="       or 
          _opr == "="        )
    {
      std::cout << "ERROR(line " << ++line_count          \
                << "): types do not match for assignment" \
                << "(lhs='" << lhs_type << "', rhs='"<< rhs_type << "')" \
                << std::endl;
      exit(1);
    }
    else if ( _opr == "=="   or
              _opr == "!="   or
              _opr == "<"    or
              _opr == "<="   or
              _opr == ">"    or
              _opr == ">="   )
    {
      std::cout << "ERROR(line " << ++line_count << "): types do not match for" \
                << "relationship operator (lhs='" << lhs_type << "', rhs='"     \
                << rhs_type << "')" << std::endl;
      exit(1);
    } 
  }

  if (  _opr == "+="       or
          _opr == "-="       or
          _opr == "/="       or
          _opr == "*="       or 
          _opr == "="        )
  {
      _type = rhs_type; //double check this  
  }
  else //relationship operators
  {
      _type = "val";
  }
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
  
  else if (_opr == "==")  {TubeIC_out << "test_equ s";}
  else if (_opr == "!=")  {TubeIC_out << "test_nequ s";} 
  else if (_opr == "<")   {TubeIC_out << "test_less s";}
  else if (_opr == "<=")  {TubeIC_out << "test_lte s";}
  else if (_opr == ">")   {TubeIC_out << "test_gtr s";}
  else if (_opr == ">=")  {TubeIC_out << "test_gte s";}
  
  else if (_opr == "=")   
  {
    if (_type == "val" or _type == "char")
    {
      TubeIC_out << "val_copy s" << rhs       \
               << " s" << lhs << std::endl; return lhs;
    }
    else if (_type == "array(char)" or _type == "array(val)")
    {
      TubeIC_out << "ar_copy a" << rhs        \
                 << " a" << lhs << std::endl; return lhs; 
    }
  }
  else if (_opr == "+=")  {TubeIC_out << "add s"<<lhs << " s"<<rhs \
                           <<" s"<<lhs <<std::endl;     return lhs;}
  else if (_opr == "-=")  {TubeIC_out << "sub s"<<lhs << " s"<<rhs \
                           <<" s"<<lhs <<std::endl;     return lhs;}
  else if (_opr == "*=")  {TubeIC_out << "mult s"<<lhs << " s"<<rhs \
                           <<" s"<<lhs <<std::endl;     return lhs;}
  else if (_opr == "/=")  {TubeIC_out << "div s"<<lhs << " s"<<rhs \
                           <<" s"<<lhs <<std::endl;     return lhs;}
  else {std::cout << "Internal Compiler ERROR!!" << std::endl;}

  int out_id = GetID();
  TubeIC_out << lhs << " s"<<rhs << " s"<<out_id << std::endl;
  return out_id;
}

//...
int TERNARY_OPR_NODE::process
  (void)
{
  int con = _children[0]->process();
  int ternary_false_id = GetLabelID();
  int ternary_end_id   = GetLabelID();
  

  TubeIC_out << "jump_if_0 s" << con<< " ternary_false_" << ternary_false_id << std::endl;
  int out_id = GetID(); 

  //executed when ternary is ture
  int if_true = _children[1]->process();
  TubeIC_out << "val_copy s" << if_true << " s" << out_id <<std::endl;
  TubeIC_out << "jump ternary_end_" << ternary_end_id << std::endl; //don't execute false ternary

  //executed when ternary is false
  TubeIC_out << "ternary_false_" << ternary_false_id << ":" << std::endl;
  int if_false = _children[2]->process();
  TubeIC_out << "val_copy s" << if_false << " s" <<out_id << std::endl; 
  
  TubeIC_out << "ternary_end_" << ternary_end_id << ":" << std::endl;
  return out_id;
}

//...
BOOL_NODE::BOOL_NODE
  (std::string opr, AST* LHS, AST* RHS)
  : _opr (opr)
{
  _children.push_back(LHS);
  _children.push_back(RHS);
  
  //check if lhs and rhs can be eval as true or false
  if (_opr == "!")
  {
    if ( RHS->GetType() != "val" ) 
    {  
      std::cout << "ERROR(line " << ++line_count << "): cannot use type '" << \
                   RHS->GetType() << "' in mathematical expressions" << std::endl;
      exit(1);
    }
  }
  else if (RHS->GetType() != "val" or LHS->GetType() != "val")
  {
    std::cout << "ERROR(line " << ++line_count << "): cannot use type '" << \
                 RHS->GetType() << "' in mathematical expressions" << std::endl;
    exit(1);
  }
  _type = "val";
}

int BOOL_NODE::process
  (void)
{ 
  if (_opr == "!")
  {
    int rhs = _children[1]->process();
    int out_id = GetID();
    TubeIC_out << "test_equ s"<< rhs << " 0 s"<< out_id << std::endl;
    return out_id;
  }
  int lhs = _children[0]->process();
  int out_id = GetID(); 
  int label_id = GetLabelID();
  if      (_opr == "&&")  
  {
    TubeIC_out << "test_nequ s"<< lhs << " 0 s" << out_id<< std::endl;
    TubeIC_out << "jump_if_0 s" << out_id<< " end_bool_" << label_id<< std::endl;
  }
  else if (_opr == "||")   
  {
    TubeIC_out << "test_nequ s"<< lhs << " 0 s" << out_id<< std::endl;
    TubeIC_out << "jump_if_n0 s" << out_id<< " end_bool_" << label_id<< std::endl;
  }
  int rhs = _children[1]->process();
  TubeIC_out << "test_nequ s"<< rhs << " 0 s" << out_id<< std::endl;
  TubeIC_out << "end_bool_" << label_id << ":" << std::endl;
  return out_id;
}

//////////////////////////////////////flow control
int IF_NODE::process
  (void)
{ 
  int con = _children[0]->process();
  int out_id = GetID(); 
  int else_label = GetLabelID();
  int end_if     = GetLabelID();

  TubeIC_out << "test_nequ s"<< con << " 0 s" << out_id<< std::endl;
  TubeIC_out << "jump_if_0 s" << out_id<< " else_" << else_label<< std::endl;
  int con_true = _children[1]->process();
  TubeIC_out << "jump end_if_" << end_if << std::endl;
  
  TubeIC_out << "else_" << else_label<< ":" << std::endl;
  if (_children[2] != NULL)
  {
    int con_false = _children[2]->process();
  }
  TubeIC_out << "end_if_" << end_if << ":" << std::endl;
  return out_id; //out_id is not used any place else
}

int WHILE_NODE::process
  (void)
{ 
  int while_id = GetLabelID();
  loop_id = while_id; // this is for break and continue statement
  TubeIC_out << "while_start_" << while_id << ":" << std::endl;
  int con = _children[0]->process();
  TubeIC_out << "jump_if_0 s" << con << " while_end_" << while_id << std::endl;
  int stmt = _children[1]->process();
  TubeIC_out << "jump while_start_" << while_id << std::endl;
  TubeIC_out << "while_end_" << while_id << ":" << std::endl;
  loop_id--;  //this is for break and continue statement
  return -1; //out_id is not used
}

int BREAK_NODE::process
  (void)
{
  TubeIC_out << "jump while_end_" << loop_id << std::endl;
}

int CONTINUE_NODE::process
  (void)
{
  TubeIC_out << "jump while_start_" << loop_id << std::endl;
}

////////////////////////////////////commands
void PRINT_CMD_NODE::AddChild 
  (AST* child)
{
  _children.push_back(child);
}

int PRINT_CMD_NODE::process
  (void)
{ 
  for (int i=0; i<_children.size(); i++)
  {
    _type = _children[i]->GetType();
    //std::cout << "print child type : "<< _type << std::endl; 

    int out_id = _children[i]->process();
    if (_type == "char")
    {
      TubeIC_out << "out_char s" << out_id << std::endl;
    }
    else if (_type == "val")
    {
      TubeIC_out << "out_val s" << out_id << std::endl;
    }
    //check if _type == string. use array tubeic commands to print them
    else if (_type == "array(char)" or _type == "string")
    {
      int len        = GetID();
      int index      = GetID();
      int pr_loop_id = GetLabelID();
      int con_id     = GetID();
      int pr_end_id  = GetLabelID();
      
      TubeIC_out << "val_copy 0 s" << index << std::endl; 
      TubeIC_out << "ar_get_siz a" << out_id << " s" << len << std::endl;
      TubeIC_out << "print_array_start_" << pr_loop_id << ":" << std::endl;
      TubeIC_out << "test_gte s" << index << " s" << len << " s" << con_id << std::endl;
      TubeIC_out << "jump_if_n0 s" << con_id << " print_array_end_" << pr_end_id << std::endl;
      TubeIC_out << "ar_get_idx a" << out_id << " s" << index << " s"  \
                 << con_id << std::endl; //con_id is used as temp scalar
      TubeIC_out << "out_char s" << con_id << std::endl; 
      TubeIC_out << "add s" << index << " 1 s" << index << std::endl;
      TubeIC_out << "jump print_array_start_" << pr_loop_id << std::endl;
      TubeIC_out << "print_array_end_" << pr_end_id << ":" << std::endl;
    }
    else if (_type == "array(val)")
    {
      int len        = GetID();
      int index      = GetID();
      int pr_loop_id = GetLabelID();
      int con_id     = GetID();
      int pr_end_id  = GetLabelID();
      
      TubeIC_out << "val_copy 0 s" << index << std::endl; 
      TubeIC_out << "ar_get_siz a" << out_id << " s" << len << std::endl;
      TubeIC_out << "print_array_start_" << pr_loop_id << ":" << std::endl;
      TubeIC_out << "test_gte s" << index << " s" << len << " s" << con_id << std::endl;
      TubeIC_out << "jump_if_n0 s" << con_id << " print_array_end_" << pr_end_id << std::endl;
      TubeIC_out << "ar_get_idx a" << out_id << " s" << index << " s"  \
                 << con_id << std::endl; //con_id is used as temp scalar
      TubeIC_out << "out_val s" << con_id << std::endl; 
      TubeIC_out << "add s" << index << " 1 s" << index << std::endl;
      TubeIC_out << "jump print_array_start_" << pr_loop_id << std::endl;
      TubeIC_out << "print_array_end_" << pr_end_id << ":" << std::endl;
    }

    else
    {
      std::cout << "Print: running error, unknow type" << std::endl;
    }
  }
  TubeIC_out << "out_char " << "'\\n'" << std::endl;
  return -1;
}

//...
RAND_CMD_NODE::RAND_CMD_NODE
  (AST* expr)
{
  _children.push_back(expr);
  if (expr->GetType() != "val")
  { 
    std::cout << "ERROR(line " << ++line_count << "): cannot use type '" << \
    expr->GetType() << "' as an argument to random" <<std::endl; 
    exit(1);
  }
  _type = expr->GetType();
}

int RAND_CMD_NODE::process
  (void)
{
  int expr_id  = _children[0]->process();
  int out_id = GetID();
  TubeIC_out << "random s" << expr_id << " s" << out_id << std::endl;
  return out_id;
}

//////////////////////////////////////extra
UMINUS_NODE::UMINUS_NODE
  (AST* expr)
{
  _children.push_back(expr);
  if (expr->GetType() != "val")
  { 
    std::cout << "ERROR(line " << ++line_count << "): cannot use type '" << \
    expr->GetType() << "' as an argument to random" <<std::endl; 
    exit(1);
  }
  _type = expr->GetType();
}

int UMINUS_NODE::process
  (void)
{
  int expr = _children[0]->process();
  int out_id = GetID();
  TubeIC_out << "mult -1 s"<< expr <<" s"<< out_id << std::endl;
  return out_id;
}

//...
void DECL_NODE::AddChild 
  (AST* child)
{
  _children.push_back(child);
}

int DECL_NODE::process
  (void)
{ 
  for (int i=0; i<_children.size(); i++)
  {
    _children[i]->process();
  }
  return -1;
}


