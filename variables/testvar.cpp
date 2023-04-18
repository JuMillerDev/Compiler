#include "variable.hpp"
#include <iostream>
#include <vector>

using namespace std;

int main(int argc, const char** argv) {
    /* Variable var;
    var.declareVariable("a");
    var.declareVariable("b");
    var.declareVariable("c");
    cout << var.isDeclared("a") << endl;
    cout << var.isDeclared("b") << endl;
    cout << var.isDeclared("c") << endl;
    var.assignMemory("a");
    var.assignMemory("b");
    var.assignMemory("c");
    cout << var.getMemoryPosition("a") << endl;
    cout << var.getMemoryPosition("b") << endl;
    cout << var.getMemoryPosition("c") << endl;

    string procedure = "swap";
    var.declareProcedure(procedure);
    var.declareVariable("a", procedure); */

    /* int var1 = 2;
    cout << var1 << endl;
    cout << &var1 << endl;
    int *var2 = &var1;
    cout << var2 << endl;
    cout << *var2 << endl; */

    Variable var;
    var.declareProcedure("swap");
    var.declareProcLocalVariable("swap", "a");
    var.declareProcLocalVariable("swap", "b");
    var.declareProcLocalVariable("swap", "c"); 

    cout << var.isProcLocalDeclared("swap", "a") << endl;
    cout << var.isProcLocalDeclared("swap", "b") << endl; 
    var.assignProcLocalMemory("swap", "a");
    var.assignProcLocalMemory("swap", "b"); 

    cout << var.getProcLocalMemoryPosition("swap", "a") << endl;
    cout << var.getProcLocalMemoryPosition("swap", "b") << endl; 

    cout << "----------------" << endl;
    

    var.declareProcedure("main");
    var.declareProcLocalVariable("main", "a");
    var.declareProcLocalVariable("main", "b");
    var.declareProcLocalVariable("main", "c");

    cout << var.isProcLocalDeclared("main", "a") << endl;
    cout << var.isProcLocalDeclared("main", "b") << endl;

    var.assignProcLocalMemory("main", "a");
    var.assignProcLocalMemory("main", "b");
    
    cout << var.getProcLocalMemoryPosition("main", "a") << endl;
    cout << var.getProcLocalMemoryPosition("main", "b") << endl;

    cout << "----------------" << endl;

    var.addNotAssignedVariable("test", "a");
    var.addNotAssignedVariable("test", "b");
    var.addNotAssignedVariable("test", "c");

    pair<string, string> var1 = var.getNotAssignedVariable(-2);

    cout << var1.first << endl;
    cout << var1.second << endl;
    

    
    return 0;
}