#ifndef SYNTXTREE_H
#define SYNTXTREE_H

#include <vector>
#include <string>
#include "SymbolTable.h"
#include <sstream>
#include <iostream>
#include <vector>

extern std::stringstream TubeIC_out;
extern std::vector<SymbolTable*> my_stack;
extern SymbolTable* symbol_table;
extern int check_var(std::string name); //used in ID_NODE processing 

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
    std::vector<AST*> _children;
    virtual void AddChild(AST*) {;}
    virtual int process() = 0;
    virtual void print() = 0;
};

class AST_ROOT: public AST
{
  public:
    AST_ROOT
      (SymbolTable* scope) { local_scope = scope;
                             did_process = false;
                           } 
    ~AST_ROOT() {for (int i=0; i < _children.size(); i++)
                 {
                  delete _children[i];
                 }
                 delete local_scope;
                }
    
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
    void print() {;}
  private:
    SymbolTable* local_scope;
    bool did_process;
};

class ID_NODE: public AST
{
  public:
    ID_NODE(std::string name):_name (name) { ; }
    ~ID_NODE() { ; }

    void print();
    int process();
  private:
    std::string _name;
};

class VAL_NODE: public AST
{
  public:
    VAL_NODE(float val):_val(val) { ; }
    ~VAL_NODE() { ; }

    void print();
    int process();
  private:
    float _val;
};

class OPR_NODE: public AST
{
  public:
    OPR_NODE(std::string, AST*, AST*);
    ~OPR_NODE()
    { 
      delete _children[0];
      delete _children[1];
      delete this;
    }
    
    int process();
    void print();
  
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
    }
    ~TERNARY_OPR_NODE()
    { 
      delete _children[0];
      delete _children[1];
      delete _children[2];
      delete this;
    }
    
    int process();
    void print();
  
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
    
    int process();
    void print();
  
  private:
    std::string _opr;
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
    
    int process();
    void AddChild (AST* child);
    void print();
  
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
    
    int process();
    void print();

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
    
    int process();
    void print();
};

class EMPTY_NODE: public AST
{
  public:
    EMPTY_NODE() { ; }
    ~EMPTY_NODE() { ; }
    int process() { ; }
    void print() { ; }
};

class DECL_NODE: public AST
{
  public:
    DECL_NODE(){;}
    ~DECL_NODE()
    { 
      for (int i=0; i<_children.size(); i++)
      {
        delete _children[i];
      }
      delete this;
    }
    
    int process();
    void AddChild (AST* child);
    void print();
  
  private: 
};



#endif /*SYNTAXTREE_H*/

