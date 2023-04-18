#include "../variables/variable.hpp"
#include "../value/value.hpp"
#include "expression.hpp"
#include <iostream>
#include <vector>


int main(int argc, const char* argv[]){
    Variable *var = new Variable();

    var->declareVariable("a");
    var->assignMemory("a");

    Value *val1 = new VarValue("a", var);
    Value *val2 = new ConstValue(3);

    Expression *expr = new MulExpr(val1, val2, var);
    vector<pair<string,long long>> v = expr->generateInstructions();

    for (int i = 0; i < v.size(); i++)
    {
        cout << v[i].first <<v[i].second << endl;
    }
}