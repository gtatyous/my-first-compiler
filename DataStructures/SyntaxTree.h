#ifndef SYNTXTREE_H
#define SYNTXTREE_H

#include <vector>
#include <string>
#include "SymbolTable.h"
#include <sstream>
#include <iostream>
#include <vector>
#include <cstdlib> 
#include <algorithm>

extern int line_count;
extern int loop_id;
extern int while_counter;
extern std::stringstream TubeIC_out;
extern std::vector<SymbolTable*> my_stack;
extern SymbolTable* symbol_table;
extern var* check_var(std::string name); //used in ID_NODE processing 

static int GetID()
{
  static int next_id = 0;
  return next_id++;
}

static int GetLabelID()
{
  static int next_label_id = 0;
  return next_label_id++;
}

class AST
{
  public:
    virtual void AddChild(AST*) {std::cout << "This node does not have children" << std::endl;}
    virtual std::string GetType() =0;
    virtual int process() = 0;
    virtual void print() = 0;
  protected:
    std::vector<AST*> _children;
    std::string _type;
};

class AST_ROOT: public AST
{
  public:
    AST_ROOT
      (SymbolTable* scope) { local_scope = scope;
                             did_process = false;
                             _type = "root";
                           } 
    ~AST_ROOT() {for (int i=0; i < _children.size(); i++)
                 {
                  delete _children[i];
                 }
                 delete local_scope;
                }
    std::string GetType() {return _type;}
    void AddChild (AST* child);
    int process() { if (did_process) return -1;
                    my_stack.push_back(local_scope);
                    symbol_table = my_stack.back();
                    for (int i=0; i<_children.size(); i++)
                    {
                      _children[i]->process();
                    }
                    my_stack.pop_back();
                    symbol_table = my_stack.back();
                    did_process = true;
                    return -1;
                  }
    void print() { ; }
  private:
    SymbolTable* local_scope;
    bool did_process;
};

class ID_NODE: public AST
{
  public:
    ID_NODE(std::string type, std::string name):_name (name) {_type = type;}
    ~ID_NODE() { ; }
   
    std::string GetType() {return _type;}
    void print() { ; }
    int process();
  private:
    std::string _name;
};

class VAL_NODE: public AST
{
  public:
    VAL_NODE(float val):_val(val) {_type = "val";}
    ~VAL_NODE() { ; }
    
    std::string GetType() {return _type;}
    void print() { ; }
    int process();
  private:
    float _val;
};

class CHAR_NODE: public AST
{
  public:
    CHAR_NODE(std::string c):_char(c) { _type = "char";}
    ~CHAR_NODE() { ; }

    std::string GetType() {return _type;}
    void print() { ; }
    int process();
  private:
    std::string _char ;
};

class ARRAY_CHAR_NODE: public AST
{
  public:
    ARRAY_CHAR_NODE(std::string s):_str(s) { _type = "array(char)";}
    ~ARRAY_CHAR_NODE() { ; }

    std::string GetType() {return _type;}
    void print() { ; }
    int process();
  private:
    std::string _str ;
};


class OPR_NODE: public AST
{
  public:
    OPR_NODE(std::string, AST* LHS, AST* RHS);
    ~OPR_NODE()
    { 
      delete _children[0];
      delete _children[1];
      delete this;
    }
    
    std::string GetType() {return _type;}
    int process();
    void print() { ; }
  
  private:
    std::string _opr;
};

class TERNARY_OPR_NODE: public AST
{
  public:
    TERNARY_OPR_NODE(AST* con, AST* if_true, AST* if_false)
    {
      _children.push_back(con);
      _children.push_back(if_true);
      _children.push_back(if_false);
      
      std::cout << "TERNARY_OPR_NOODE: does not check for type" <<std::endl;
      //check if con evaluates to condition
      //check if if_true and i_false have to have the same type
    }
    ~TERNARY_OPR_NODE()
    { 
      delete _children[0];
      delete _children[1];
      delete _children[2];
      delete this;
    }
    std::string GetType() {return _type;}
    int process();
    void print() { ; }
  
  private:
};


class BOOL_NODE: public AST
{
  public:
    BOOL_NODE(std::string, AST*, AST*);
    ~BOOL_NODE()
    { 
      delete _children[0];
      delete _children[1];
      delete this;
    }
    
    std::string GetType() {return _type;}
    int process();
    void print() { ; }
  
  private:
    std::string _opr;
};

class IF_NODE: public AST
{
  public:
    IF_NODE (AST* con, AST* con_true, AST* con_false) 
    { _children.push_back(con);
      _children.push_back(con_true);
      _children.push_back(con_false);
      if (con->GetType() != "val")
      {
        std::cout << "ERROR(line " << ++line_count << "): condition for if statements " \
                  << "must evaluate to type val" << std::endl;
        exit(1);
      }
    }
    IF_NODE ()
    { 
      delete _children[0];
      delete _children[1];
      delete _children[2];
      delete this;
    }
    std::string GetType() {std::cout << "IF_NODE: has no type" <<std::endl;}
    int process();
    void print() { ; }
};

class WHILE_NODE: public AST
{
  public:
    WHILE_NODE (AST* con, AST* stmt) 
    { 
      _children.push_back(con);
      _children.push_back(stmt);
      if (con->GetType() != "val")
      {
        std::cout << "ERROR(line " << ++line_count        \
                  << "): condition for while statements " \
                  << "must evaluate to type val" << std::endl;
        exit(1);
      }
    }
    WHILE_NODE ()
    { 
      delete _children[0];
      delete _children[1];
      delete this;
    }
    std::string GetType() {std::cout << "WHILE_NODE: has no type" <<std::endl;}
    int process();
    void print() { ; }
};

class BREAK_NODE: public AST
{
  public:
    BREAK_NODE () { if (while_counter == 0) 
                    {
                      std::cout << "ERROR(line " << ++line_count   \
                                << "): 'break' command used outside of any loop" \
                                << std::endl; 
                      exit(1);
                    }
                  } 
    ~BREAK_NODE () { ; }
    std::string GetType() {std::cout << "BREAK_NODE: has no type" <<std::endl;}
    int process();
    void print() { ; }
};

class CONTINUE_NODE: public AST
{
  public:
    CONTINUE_NODE () { if (while_counter == 0) 
                    {
                      std::cout << "ERROR(line " << ++line_count   \
                                << "): 'continue' command used outside of any loop" \
                                << std::endl; 
                      exit(1);
                    }
                  } 
    ~CONTINUE_NODE () { ; }
    std::string GetType() {std::cout << "CONTINUE_NODE: has no type" <<std::endl;}
    int process();
    void print() { ; }
};


class PRINT_CMD_NODE: public AST
{
  public:
    PRINT_CMD_NODE(){;}
    ~PRINT_CMD_NODE()
    { 
      for (int i=0; i<_children.size(); i++)
      {
        delete _children[i];
      }
      delete this;
    }
    
    std::string GetType() {std::cout<< "Print doesn't have type" << std::endl;}
    int process();
    void AddChild (AST* child);
    void print() { ; }
  
  private: 
};

class RAND_CMD_NODE: public AST
{
  public:
    RAND_CMD_NODE(AST*);
    ~RAND_CMD_NODE()
    { 
      delete _children[0];
      delete this;
    }
    std::string GetType() {return _type;}
    int process();
    void print() { ; }

  private: 
};

class UMINUS_NODE: public AST
{
  public:
    UMINUS_NODE(AST*);
    ~UMINUS_NODE()
    { 
      delete _children[0];
      delete _children[1];
      delete this;
    }
    std::string GetType() {return _type;}
    int process();
    void print() { ; }
};

class EMPTY_NODE: public AST
{
  public:
    EMPTY_NODE() { ; }
    ~EMPTY_NODE() { ; }
    std::string GetType() {std::cout << "EMPTY_NODE: has no type"<<std::endl;}
    int process() { ; }
    void print() { ; }
};

class DECL_NODE: public AST
{
  public:
    DECL_NODE(std::string t){_type = t;}
    ~DECL_NODE()
    { 
      for (int i=0; i<_children.size(); i++)
      {
        delete _children[i];
      }
      delete this;
    }
    
    std::string GetType() {std::cout << "DECL_NODE:has no type" << std::endl;}
    int process();
    void AddChild (AST* child);
    void print() { ; }
};

#endif /*SYNTAXTREE_H*/

