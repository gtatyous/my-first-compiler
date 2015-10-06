#ifndef SYNTXTREE_H
#define SYNTXTREE_H

#include <vector>
#include <string>
#include "SymbolTable.h"

extern SymbolTable symbol_table;
static int GetID()
{
  static int next_id = 0;
  return next_id++;
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
    AST_ROOT() {;}
    ~AST_ROOT() {for (int i=0; i < _children.size(); i++)
                 {
                  delete _children[i];
                 }
                }
    
    void AddChild (AST* child);
    int process() {for (int i=0; i<_children.size(); i++)
                   {
                    _children[i]->process();
                   }
                  }
    void print() {;}
    
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

class CMD_NODE: public AST
{
  public:
    CMD_NODE(AST* e){_children.push_back(e);}
    ~CMD_NODE()
    { 
      delete _children[0];
      delete this;
    }
    
    int process();
    void print();
  
  private:
  
};


#endif /*SYNTAXTREE_H*/

