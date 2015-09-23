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

#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_

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
        ~Node()=default;
};

class LINKEDLIST {
    public:
      LINKEDLIST(): root(nullptr) {head=root;}
      ~LINKEDLIST();
      void insert(string);
      bool search(string);
      void print();

    private:
    Node *root;
    Node *head;
    void destroyTree(Node*);
};

#endif    /* LINKEDLIST_H_ */
