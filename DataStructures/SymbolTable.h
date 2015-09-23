#ifndef SYMBOLTABLE_H_
#define SYMBOLTABLE_H_

#include <string>
#include <map>

using std::string;


typedef struct var
{
  public:
    var(string name): name(name) {}
    ~var()=default;
    string name;
    int scope;
    bool init;
    string type;
    int line;
    void* value;
    int mem_location;
} var;


class SymbolTable
{
  public:
    SymbolTable() {}
    ~SymbolTable();
    bool is_declared(string);
    void insert(string);
    var* search(string); 
    void print();

  private:
    std::map<string, var*> var_info;
};


#endif
