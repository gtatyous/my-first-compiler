/******************************************************
 ** FILE: bst.h
 **
 ** CLASS:
 ** CSE331
 **
 ** Homework 03 / Project 02
 **
 ** AUTHOR:
 ** Yousef Gtat
 **
 ** CREATION DATE:
 ** 02/27/2015
 **
 ** NOTES:
 ** Due to extensive use of recursive functions, few
 ** private function implemented to utilize them in the
 ** original method to achieve the required task
 *******************************************************/

#ifndef BST_H_
#define BST_H_

class Node {
    public:
        int key;
        Node *left;
        Node *right;
        Node(int k): key(k), left(nullptr), right(nullptr) {}
        ~Node()=default;
};

class BST {
    public:
      BST(): root(nullptr) {}
      ~BST();
      void insert(int);
      bool search(int);
      void preOrder();
      void inOrder();
      void postOrder();
    
    private:
    Node *root;
    void addNode(int, Node*);
    void print_preOrder(Node*);
    void print_inOrder(Node*);
    void print_postOrder(Node*);
    void destroyTree(Node*);
};

#endif
