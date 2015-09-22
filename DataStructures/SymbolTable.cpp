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


bool rec_search
  (string var, Node* inNode)
{
  static bool match = false;
  if (inNode == nullptr) {}
  else if (inNode->name == var) match = true;
  else rec_search(var, inNode->next);
  return match;
}

bool SymbolTable::search
  (string var)
{
  bool match = rec_search(var, root);
  return match;
}

int rec_print
  (Node* inNode)
{
  if (inNode == nullptr) return 0;
  cout<< inNode->name << endl;
  rec_print(inNode->next);
  return 1;
}

void SymbolTable::print
  (    )
{
  rec_print(root);
}

SymbolTable::~SymbolTable()
{
    destroyTree (root);
}

void SymbolTable::destroyTree (Node* inNode)
{
    if (inNode != nullptr)
    {
        destroyTree(inNode->next);
        delete inNode;
    }
}





