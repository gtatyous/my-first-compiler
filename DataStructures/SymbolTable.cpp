/******************************************************
 ** FILE: SymbolTable.cpp
 **
 ** CLASS:
 ** CSE450
 **
 ** AUTHOR:
 ** Yousef Gtat
 **
 ** CREATION DATE:
 ** 09/21/2015
 **
 ** NOTES:
 *******************************************************/

#include "SymbolTable.h"
#include <iostream>

using std::cout;
using std::endl;

//Create initial root Node and then insert to the BST
void SymbolTable::insert 
  (string var)
{
  if (root==nullptr)
  {
    root = new Node(var);
    head = root;
  }
  else
  {
    head->next = new Node(var);
    head = head->next;
  }
}

bool SymbolTable::search
  (string var, Node* inNode)
{
  static match = false;
  if (inNode == nulptr) return;
  else if (inNode->name == var) match = true;
  else search(var, inNode->next);
}

void SymbolTable::print
  (Node* inNode)
{
  if (inNode == nulptr) return;
  cout<< inNode->name << endl;
  print(inNode->next);
}

SymbolTable::~SymbolTable()
{
    destroyTree (root);
}

void BST::destroyTree (Node* inNode)
{
    if (inNode != nullptr)
    {
        destroyTree(inNode->next);
        delete inNode;
    }
}





