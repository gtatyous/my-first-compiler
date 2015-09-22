/******************************************************
 ** FILE: bst.cpp
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

#include "bst.h"
#include <iostream>

using std::cout;
using std::endl;

//Create initial root Node and then insert to the BST
void BST::insert (int key)
{
    if (root==nullptr)
    {
        root = new Node(key);
    }
    else
    {
        addNode(key, root);
    }
}

//Always start from the root node, then recursively
//move the next node
void BST::addNode (int key, Node *inNode)
{
    if (key > inNode->key)
    {
        if (inNode->right == nullptr)
        {
            inNode->right = new Node(key);
        }
        else
        {
            addNode(key, inNode->right);
        }
    }
    
    else if (key < inNode->key)
    {
        if (inNode->left == nullptr)
        {
            inNode->left = new Node(key);
        }
        else
        {
            addNode(key, inNode->left);
        }
    }

}

//Cannot use this operator if tree is empty
//Otherwise prints tree using preorder triversal
void BST::preOrder()
{
    if (root == nullptr)
    {
        cout << "Tree is empty!" <<endl;
    }
    else
    {
        cout << "preorder: ";
        print_preOrder(root);
        cout << endl;
    }
}

void BST::print_preOrder(Node* inNode)
{
    if (inNode != nullptr)
    {
        cout << inNode->key << " ";
        print_preOrder(inNode->left);
        print_preOrder(inNode->right);
    }
}

//Cannot use this operator if tree is empty
//Otherwise prints tree using inorder triversal
void BST::inOrder()
{
    if (root == nullptr)
    {
        cout << "Tree is empty!" <<endl;
    }
    else
    {
        cout << "inorder: ";
        print_inOrder(root);
        cout << endl;
    }
}

void BST::print_inOrder(Node* inNode)
{
    if (inNode != nullptr)
    {
        print_inOrder(inNode->left);
        cout << inNode->key << " ";
        print_inOrder(inNode->right);
    }
}


//Cannot use this operator if tree is empty
//Otherwise prints tree using postorder triversal
void BST::postOrder()
{
    if (root == nullptr)
    {
        cout << "Tree is empty!" <<endl;
    }
    else
    {
        cout << "postorder: ";
        print_postOrder(root);
        cout << endl;
    }
}

void BST::print_postOrder(Node* inNode)
{
    if (inNode != nullptr)
    {
        print_postOrder(inNode->left);
        print_postOrder(inNode->right);
        cout << inNode->key << " ";
    }
}

//Destructor of BST to take care of the allocated memory
BST::~BST()
{
    destroyTree (root);
}

void BST::destroyTree (Node* inNode)
{
    if (inNode != nullptr)
    {
        destroyTree(inNode->left);
        destroyTree(inNode->right);
        delete inNode;
    }
}





