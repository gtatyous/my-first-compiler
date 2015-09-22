/******************************************************
 ** FILE: SymbolTable.h
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

#ifndef SYMBOLTABLE_H_
#define SYMBOLTABLE_H_

#include <string>
using std::string;

class Node {
    public:
        string name;
        Node *next;
        bool scope;
        bool init;
        int mem_position;
        Node(string var): name(var), next(nullptr) {}
        ~Node();
};

class SymbolTable {
    public:
      SymbolTable(): root(nullptr) {head=root}
      ~SymbolTable();
      void insert(string);
      bool search(string, Node* inNode=root);
      void print();

    private:
    Node *root;
    Node *head;
    void destroyTree(Node*);
};

#endif
