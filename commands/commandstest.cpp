#include "command.hpp"
#include "../value/value.hpp"
#include "../expressions/expression.hpp"
#include "../conditions/condition.hpp"
#include "../variables/variable.hpp"
//#include "../assembly/assembly.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <typeinfo>


using namespace std;

int main(int argc, const char* argv[]) {
    Variable *var = new Variable();
    var->declareVariable("a");
    var->assignMemory("a");
    var->declareVariable("c");
    var->assignMemory("c");
    var->declareVariable("b");
    int assemblerLineNo = -1;

    Value *val = new VarValue("a", var);
    //Value *val2 = new ConstValue(3);
    Value *val2 = new VarValue("c", var);


    Expression *expr = new MulExpr(val2, val, var);
    string b = "b";
    var->declareVariable(b);
    Command *cmd = new Assign(b, expr, var);
    cout << var->getMemoryPosition("a") << endl;
    cout << var->getMemoryPosition("b") << endl;
    cout << var->getMemoryPosition("c") << endl;

    vector<pair<string,long long>> l = cmd->generateInstructions();
    cout << "assembleLineNo: " << assemblerLineNo << endl;
    cout << "Instructions:" << endl;
    
    for (int i = 0; i < l.size(); i++)
    {
        cout << l[i].first << l[i].second << endl;
    }

    cout << "Testing Repeat Command" << endl;
    Condition *cond = new Eq(val, val2, var->getTempValueMemoryPosition());
    Command *cmd2 = new Repeat(cond, l);
    vector<pair<string,long long>> l2 = cmd2->generateInstructions();

    for (int i = 0; i < l2.size(); i++)
    {
        cout << l2[i].first << l2[i].second << endl;
    }


    /* cout << l.back() << endl;
    l.back() += " 2";
    cout << l.back() << endl;

    var->printVariables();

    list<string> command = {"1st","scnd"};

    l.clear();
    l.splice(l.end(), command);

    it = l.begin();
    while (it != l.end())
    {
        cout << *it << endl;
        it++;
    }

    bool testbool = true;
    int testint = 2;

    cout << testint + testbool << endl; */
    
}