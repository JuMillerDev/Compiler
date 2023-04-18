#pragma once
#include "../expressions/expression.hpp"
#include "../variables/variable.hpp"
#include "../conditions/condition.hpp"
#include "commands.hpp"
#include <vector>
#include <string>

using namespace std;

class Command{
private:
    vector<string> cmds = {"PUT","ADD ","SUB ","JUMP ","JPOS ","JZERO ","SET ","STORE ","LOAD "};
public:
    virtual ~Command() = 0;
    virtual vector<pair<string,long long>> generateInstructions() = 0;
    void assignCheckVariable(string procName,string identifier, Variable *var, Expression *expression, int line);
    void readCheckVariable(string procName,bool isProcHead,string identifier, Variable *var);
    void callCheckVariable(string procName,string identifier, Variable *var);
    void writeCheckVariable(string procName,bool isProcHead,string identifier, Variable *var);
    void checkLoopVariables(string procName, Commands *commands, Variable *var);
    void checkCommandsVariables(string procName, Commands *commands, Variable *var, int line);
};

//when doing if 

class If : public Command{
    private:
        Condition *condition;
        vector<pair<string,long long>> commands;
    public:
        If(Condition *condition, Commands *commands, Variable *var, string procName, int line);
        ~If();
        vector<pair<string,long long>> generateInstructions() override;
};

class If_Else : public Command{
    private:
        Condition *condition;
        vector<pair<string,long long>> commands;
        vector<pair<string,long long>> elseCommands;
    public:
        If_Else(Condition *condition, Commands *commands, Commands *elseCommands, Variable *var, string procName, int line);
        ~If_Else();
        vector<pair<string,long long>> generateInstructions() override;
};

class Repeat : public Command{
    private:
        Condition *condition;
        vector<pair<string,long long>> commands;
        vector<pair<string,long long>> instructions;
        vector<string> warnedVariables;
    public:
        Repeat(Condition *condition, Commands *commands, Variable *var, string procName);
        ~Repeat();
        vector<pair<string,long long>> generateInstructions() override;

};

class While : public Command{
    private:
        Condition *condition;
        vector<pair<string, long long>> commands;
        vector<pair<string, long long>> instructions;
        vector<string> warnedVariables;
    public:
        While(Condition *condition, Commands *commands, Variable *var, string procName);
        ~While();
        vector<pair<string,long long>> generateInstructions() override;
};  

class Assign : public Command{
    private:
        Expression *expression;
        Variable *var;
        string identifier;
        bool isProcHead = false;
        string procName = "";
    public:
        //Assign(string identifier, Expression *expression, Variable *var);
        Assign(string identifier,Expression *expression, Variable *var, string procName, int line);
        ~Assign();
        vector<pair<string,long long>> generateInstructions() override;
};

class Read : public Command{
    private:
        Variable *var;
        string identifier;
        vector<pair<string,long long>> instructions;
        bool isProcHead = false;
        string procName = "";
    public:
        //Read(string identifier, Variable *var);
        Read(string identifier,string procName, Variable *var);
        ~Read();
        vector<pair<string,long long>> generateInstructions() override;

};

class Write : public Command{
    private:
        Variable *var;
        Value *value;
        vector<pair<string,long long>> instructions;
        bool isProcHead = false;
        string procName = "";
    public:
        //Write(Value *value, Variable *var);
        Write(Value *value, Variable *var, string procName);
        ~Write();
        vector<pair<string,long long>> generateInstructions() override;
};

class Call_Proc : public Command{
    private:
        string procName;
        string currentProc;
        Variable *var;
        vector<string> assignedValues;
        vector<pair<string,long long>> pointerValues;
        vector<pair<string,long long>> instructions;
    public:
        Call_Proc(string procName, vector<string> assignedValues , Variable *var, string currentProc);
        ~Call_Proc();
        vector<pair<string,long long>> generateInstructions() override;
}; 

