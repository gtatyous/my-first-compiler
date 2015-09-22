#include "SymbolTable.h"
#include <iostream>


int main ()
{

  SymbolTable st;
  st.insert("my_var1");
  st.insert("my_var2");
  st.print();
  std::cout << st.search("my_var1") <<std::endl;

  return 0;



}
