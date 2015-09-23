#include "SymbolTable.h"
#include <iostream>
  
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
    return true;
  }
  return false;
}

void SymbolTable::insert
  (string name)
{
  if (is_declared(name))
  {
    /* Skip without inserting */
  }
  else
  {
    var_info[name] = new var(name);
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

