#ifndef SYMBOLTABLE_H_
#define SYMBOLTABLE_H_

#include <string>
#include <map>

using std::string;


class var
{
  public:
    var() {}
    ~var() {}
    
    string name;
    string type;
    bool init;
    int line;
    int mem_location;
  
  private:
    int scope;
    void* init_value;
};


class SymbolTable
{
  public:
    SymbolTable();
    ~SymbolTable();
    bool is_declared(string name);
    void insert(string name);
    var* search(string name); 
    void print();

  private:
    std::map<string, var*> var_info;
};


#endif
