#include "SymbolTable.h"
#include "SyntaxTree.h" 
#include <iostream>
  
SymbolTable::SymbolTable
  (  )
{
  /* do nothing for now*/
}

SymbolTable::~SymbolTable
  (  )
{
  std::map<string, var*>::iterator it;
  for (it=var_info.begin(); it!=var_info.end(); it++)
  {
    delete it->second;
  }
}

bool SymbolTable::is_declared
  (string name)
{
  if (var_info.count(name))
  {
    //can check for the type here
    return true;
  }
  return false;
}

void SymbolTable::insert
  (string name, string type, int line_count, int scope_level)
{
  if (is_declared(name))
  {
    /* Skip without inserting */
  }
  else
  {
    var* varptr = new var();
    var_info[name] = varptr;
    var_info[name]->name = name;
    var_info[name]->type = type;
    var_info[name]->line = line_count;
    var_info[name]->scope = scope_level; 
    var_info[name]->id = GetID();
  }
}

var* SymbolTable::search
  (string name)
{
  /* users need to use is_declared member function to check
   * if the name exist or not
   */
  return var_info[name];
}


void SymbolTable::print
  (  )
{
  std::map<string, var*>::iterator it;
  for (it=var_info.begin(); it!=var_info.end(); it++)
  {
    std::cout << it->first << ", ";
  }
    std::cout << std::endl;
}

