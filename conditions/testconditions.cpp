#include "condition.hpp"
#include "../value/value.hpp"
#include "../variables/variable.hpp"

#include <string>
#include <vector>
#include <iostream>

using namespace std;

int main(int argc, const char* argv[]){
    Variable *var = new Variable();
    var->declareVariable("a");
    var->assignMemory("a");
    var->declareVariable("b");
    var->assignMemory("b");

    Value *val1 = new VarValue("a", var);
    Value *val2 = new VarValue("b", var);

    Value *val3 = new ConstValue(2);
    Value *val4 = new ConstValue(3);

    Condition *cond = new Eq(val1, val2, var->getTempValueMemoryPosition()); 
    vector<pair<string,long long>> v = cond->generateInstructions();

    for (int i = 0; i < v.size(); i++)
    {
        cout << v[i].first <<v[i].second << endl;
    }
}