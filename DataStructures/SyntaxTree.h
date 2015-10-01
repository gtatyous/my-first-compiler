#ifndef SYNTXTREE_H
#define SYNTXTREE_H

#include <iostream>
#include <vector>
#include <string>

class AST
{
  public:
    AST(){}
    ~AST() {}

    AddChild(AST* c) {_children.push_back(a);}
    //print(); //for loop through all _children
  private:
    std::vector<AST*> _children
};


class ID_NODE: public AST
{
  public:
    ID_NODE(std::string name):_name(name){}
    ~ID_NODE(){}
  private:
    std::string _name;
};

class VAL_NODE: public AST
{
  public:
    VAL_NODE(int val):_val(val){}
    ~VAL_NODE(){}
  private:
    int _val;
};

class OPR_NODE: public AST
{
  public:

  private:

};

#endif /*SYNTAXTREE_H*/

