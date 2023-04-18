#include "value.hpp"
#include "../variables/variable.hpp"
#include <iostream>

using namespace std;

int main(){

    Value *val = new ConstValue(2);
    cout << val->isConst() << endl;
    cout << typeid(val).name() << endl;
    Value *val2 = new VarValue("a", new Variable());
    cout << typeid(val2).name() << endl;

    Variable *var = new Variable();
    var->declareVariable("a");
    var->assignMemory("a");

    Value *val3 = new VarValue("a", var);
}