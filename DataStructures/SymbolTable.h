#ifndef SYMBOLTABLE_H_
#define SYMBOLTABLE_H_

#include <string>
#include <map>

using std::string;


class var
{
  public:
    var():id(-1) {}
    ~var() {}
    
    string name;
    string type;
    int line;
    int scope;
    int id;
    bool init;
    int mem_location;
    void* init_value;
};


class SymbolTable
{
  public:
    SymbolTable();
    ~SymbolTable();
    bool is_declared(string name);
    void insert(string name, string type, int line, int scope);
    var* search(string name); 
    void print();

  private:
    std::map<string, var*> var_info;
};


#endif
