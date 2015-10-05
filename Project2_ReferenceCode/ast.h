#ifndef AST_H
#define AST_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "symbol_table.h"

class ASTNode_Base {
protected:
  int type;                              // What type should this node pass up?
  std::vector<ASTNode_Base *> children;  // What sub-trees does this node have?

  // NOTE: Other basic AST information should go here, as needed.
  
public:
  ASTNode_Base(int in_type) : type(in_type) { ; }
  virtual ~ASTNode_Base() {
    for (int i = 0; i < children.size(); i++) {
      delete children[i];
    }
  }

  int GetType() { return type; }

  ASTNode_Base * GetChild(int id) { return children[id]; }
  int GetNumChildren() { return children.size(); }
  void AddChild(ASTNode_Base * in_child) { children.push_back(in_child); }
  void TransferChildren(ASTNode_Base * in_node);

  // Process a single node's calculations and return a variable pointer that
  // represents the result.  Call child nodes recursively....

  virtual void Debug(int indent=0);
  virtual std::string GetName() { return "ASTNode_Base"; }
};



// This node is built as a placeholder in the AST.
class ASTNode_Temp : public ASTNode_Base {
public:
  ASTNode_Temp(int in_type) : ASTNode_Base(in_type) { ; }
  ~ASTNode_Temp() { ; }
  virtual std::string GetName() {
    return "ASTNode_Temp (under construction)";
  }
};

// Root...
class ASTNode_Root : public ASTNode_Base {
public:
  ASTNode_Root() : ASTNode_Base(Type::VOID) { ; }

  virtual std::string GetName() { return "ASTNode_Root (container class)"; }
};

// Leaves...

class ASTNode_Variable : public ASTNode_Base {
private:
  tableEntry * var_entry;
public:
  ASTNode_Variable(tableEntry * in_entry)
    : ASTNode_Base(in_entry->GetType()), var_entry(in_entry) {;}

  tableEntry * GetVarEntry() { return var_entry; }

  virtual std::string GetName() {
    std::string out_string = "ASTNode_Variable (";
    out_string += var_entry->GetName();
    out_string += ")";
    return out_string;
  }
};

class ASTNode_Literal : public ASTNode_Base {
private:
  std::string lexeme;                 // When we print, how should this node look?
public:
  ASTNode_Literal(int in_type, std::string in_lex)
    : ASTNode_Base(in_type), lexeme(in_lex) { ; }  

  virtual std::string GetName() {
    std::string out_string = "ASTNode_Literal (";
    out_string += lexeme;
    out_string += ")";
    return out_string;
  }
};

// Math...

class ASTNode_Assign : public ASTNode_Base {
public:
  ASTNode_Assign(ASTNode_Base * lhs, ASTNode_Base * rhs)
    : ASTNode_Base(lhs->GetType())
  {  children.push_back(lhs);  children.push_back(rhs);  }
  ~ASTNode_Assign() { ; }

  virtual std::string GetName() { return "ASTNode_Assign (operator=)"; }
};

class ASTNode_Math1_Minus : public ASTNode_Base {
public:
  ASTNode_Math1_Minus(ASTNode_Base * child) : ASTNode_Base(Type::VAL)
  { children.push_back(child); }
  ~ASTNode_Math1_Minus() { ; }

  virtual std::string GetName() { return "ASTNode_Math1_Minus (unary -)"; }
};

class ASTNode_Math2 : public ASTNode_Base {
protected:
  char math_op;
public:
  ASTNode_Math2(ASTNode_Base * in1, ASTNode_Base * in2, char op);
  virtual ~ASTNode_Math2() { ; }

  virtual std::string GetName() {
    std::string out_string = "ASTNode_Math2 (operator";
    out_string += math_op;
    out_string += ")";
    return out_string;
  }
};

class ASTNode_Print : public ASTNode_Base {
public:
  ASTNode_Print() : ASTNode_Base(Type::VOID) { ; }
  virtual ~ASTNode_Print() {;}

  virtual std::string GetName() { return "ASTNode_Print (print command)"; }
};

#endif
