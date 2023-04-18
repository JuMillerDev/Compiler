#include "command.hpp"
#include <iostream>
#include <list>
#include <string>

using namespace std;

Command::~Command(){}

void Command::checkLoopVariables(string procName, Commands *commands, Variable *var){
    vector<string> warnedVariables;
    vector<pair<string,long long>> instructions = commands->getInstructions();

    for(int i = 0;i<instructions.size();i++){
        if(!(instructions[i].first == "JUMP " || instructions[i].first == "JPOS " || instructions[i].first == "JZERO ") && var->getNotAssignedVariable(instructions[i].second).second != ""){
            pair<string,string> notAssignedVariable = var->getNotAssignedVariable(instructions[i].second);
            if(find(warnedVariables.begin(), warnedVariables.end(), notAssignedVariable.second) == warnedVariables.end()){
                cout << "Warning: Variable " << notAssignedVariable.second << " may be unassigned" << endl;
                if(procName != "null"){
                    if(var->isProcLocalAssigned(notAssignedVariable.first, notAssignedVariable.second)){
                        instructions[i].second = var->getProcLocalMemoryPosition(notAssignedVariable.first, notAssignedVariable.second);
                    }else{
                        var->assignProcLocalMemory(notAssignedVariable.first, notAssignedVariable.second);
                    }
                }
                else{
                    if(var->isAssigned(notAssignedVariable.second)){
                        instructions[i].second = var->getMemoryPosition(notAssignedVariable.second);
                    }else{
                        var->assignMemory(notAssignedVariable.second);
                    }
                }
                warnedVariables.push_back(notAssignedVariable.second);
            }
        }
    } 
}

void Command::checkCommandsVariables(string procName, Commands *commands, Variable *var, int line){
    vector<pair<string,long long>> instructions = commands->getInstructions();

    for(int i = 0; i<instructions.size();i++){
        if(instructions[i].first == "JUMP " || instructions[i].first == "JPOS " || instructions[i].first == "JZERO " && instructions[i].second == -1){    
            //throw ("Error: Jump to undefined line: " + to_string(line));    
            var->setErrorLine(line);
        }  
    }
}


void Command::writeCheckVariable(string procName,bool isProcHead,string identifier, Variable *var){
    if(isProcHead || identifier == ""){
        return;   
    }
    else if(procName != "null"){
        if(!var->isProcLocalDeclared(procName, identifier)){
            //cout << "Error: Variable " << identifier << " is not declared" << endl;
            throw ("Error: Variable " + identifier + " is not declared");
        }
        else if (!var->isProcLocalAssigned(procName, identifier)){
            throw ("Error: variabble is not assigned: " + identifier);
        }
    }
    else{
        if(!var->isDeclared(identifier)){
            //cout << "Error: Variable " << identifier << " is not declared" << endl;
            throw ("Error: Variable " + identifier + " is not declared");
        }
        else if (!var->isAssigned(identifier)){
            throw ("Error: variable " + identifier + " is not assigned");
        }
    }
}

void Command::callCheckVariable(string procName, string identifier, Variable *var){
    if(var->isProcHeadAssigned(procName, identifier)){
        //throw ("Error: Procedure head variable " + identifier + " cannot be used in call statement");
        return;
    }
    else if(procName != "null"){
        if(!var->isProcLocalDeclared(procName, identifier)){
            //cout << "Error: Variable " << identifier << " is not declared" << endl;
            throw ("Error: Variable " + identifier + " is not declared");
        }
        else if (!var->isProcLocalAssigned(procName, identifier)){
            var->assignProcLocalMemory(procName, identifier);
        }
    }
    else{
        if(!var->isDeclared(identifier)){
            //cout << "Error: Variable " << identifier << " is not declared" << endl;
            throw ("Error: Variable " + identifier + " is not declared");
        }
        else if (!var->isAssigned(identifier)){
            var->assignMemory(identifier);
        }
    }
}

void Command::assignCheckVariable(string procName,string identifier, Variable *var, Expression *expr,int line){
    
    if(expr->getLeft()->getValue() == -1){
        var->setErrorLine(line);
        var->addNotAssignedVariable(procName, expr->getLeft()->getIdentifier());
    }else if (expr->getRight()->getValue() == -1){
        //cout << "Error: Variable " << expr->getRight()->getIdentifier() << " is not assigned" << endl;
        var->setErrorLine(line);
        var->addNotAssignedVariable(procName, expr->getRight()->getIdentifier());
    }

    if(var->isProcHeadAssigned(procName, identifier)){
        return;
    }
    else if(procName != "null"){
        if(!var->isProcLocalDeclared(procName, identifier)){
            cout << "Error: Variable " << identifier << " is not declared" << endl;
            throw ("Error: Variable " + identifier + " is not declared");
        }
        else if (!var->isProcLocalAssigned(procName, identifier)){
            var->assignProcLocalMemory(procName, identifier);
        } 
    
    }
    else{
        if(!var->isDeclared(identifier)){
            cout << "Error: Variable " << identifier << " is not declared" << endl;
            throw ("Error: Variable " + identifier + " is not declared");
        }
        else if (!var->isAssigned(identifier)){
            var->assignMemory(identifier);
        }

    }
}

void Command::readCheckVariable(string procName, bool isProcHead,string identifier, Variable *var){
    if(isProcHead){
        throw "Error: Cannot read procedure head variable";    
    }
    else if(procName != "null"){
        if(!var->isProcLocalDeclared(procName, identifier)){
            cout << "Error: Variable " << identifier << " is not declared" << endl;
            throw ("Error: Variable " + identifier + " is not declared");
        }
        else if (!var->isProcLocalAssigned(procName, identifier)){
            var->assignProcLocalMemory(procName, identifier);
        }
    }
    else{
        if(!var->isDeclared(identifier)){
            cout << "Error: Variable " << identifier << " is not declared" << endl;
            throw ("Error: Variable " + identifier + " is not declared");
        }
        else if (var->isAssigned(identifier)){
            throw ("Error: Cannot read assigned variable: " + identifier);
        }
    }
}

If::If(Condition *condition, Commands *commands,Variable *var, string procName, int line){
    this->condition = condition;
    this->commands = commands->getInstructions();

    checkCommandsVariables(procName,commands,var,line);
}

If::~If(){
    delete condition;
}

vector<pair<string,long long>> If::generateInstructions(){
    vector<pair<string,long long>> condInstr = condition->generateInstructions();
    //long long els = assemblerLineNo + condInstr.size() + commands.size();

    for(int i = 0; i < condInstr.size(); i++){
        /* assemblerLineNo++;
        if ((condInstr[i].first == "JZERO " || condInstr[i].first == "JUMP " || condInstr[i].first == "JPOS ") && condInstr[i].second != 0){
            condInstr[i].second = assemblerLineNo + condInstr[i].second;
        } */
        if (condInstr[i].first == "JZERO " || condInstr[i].first == "JUMP " || condInstr[i].first == "JPOS " && condInstr[i].second == 0){
            condInstr[i].second = (condInstr.size() - (i+1)) + commands.size() + 1;
        }
    }

    condInstr.insert(condInstr.end(), commands.begin(), commands.end());
    //assemblerLineNo += condInstr.size();
    return condInstr;
    //set assemblyline in assebly.cpp
}

If_Else::If_Else(Condition *condition, Commands *commands, Commands *elseCommands, Variable *var, string procName, int line){
    this->condition = condition;
    this->commands = commands->getInstructions();
    this->elseCommands = elseCommands->getInstructions();

    checkCommandsVariables(procName,commands,var,line);
    checkCommandsVariables(procName,elseCommands,var,line);
}

If_Else::~If_Else(){
    delete condition;
}

vector<pair<string,long long>> If_Else::generateInstructions(){
    vector<pair<string,long long>> condInstr = condition->generateInstructions();
    //long long els = assemblerLineNo + condInstr.size() + commands.size() + 1;
    //long long end = els + elseCommands.size();

    for(int i = 0; i < condInstr.size(); i++){
        //assemblerLineNo++;
        /* if ((condInstr[i].first == "JZERO " || condInstr[i].first == "JUMP " || condInstr[i].first == "JPOS ") && condInstr[i].second != 0){
            condInstr[i].second = assemblerLineNo + condInstr[i].second;
        } */
        if ((condInstr[i].first == "JZERO " || condInstr[i].first == "JUMP " || condInstr[i].first == "JPOS ") && condInstr[i].second == 0){
            condInstr[i].second = (condInstr.size() - (i+1)) + commands.size() + 2; //+1 for jump to end,
        }
    }

    condInstr.insert(condInstr.end(), commands.begin(), commands.end());
    condInstr.push_back(make_pair("JUMP ", elseCommands.size()+1));
    condInstr.insert(condInstr.end(), elseCommands.begin(), elseCommands.end());
    //assemblerLineNo += condInstr.size();
    return condInstr;
    //set assemblyline in assebly.cpp
}

Repeat::Repeat(Condition *condition, Commands *commands,Variable *var, string procName){
    this->condition = condition;
    this->commands = commands->getInstructions();

    checkLoopVariables(procName, commands, var);
}

Repeat::~Repeat(){
    delete condition;
}

vector <pair<string,long long>> Repeat::generateInstructions(){
    vector<pair<string,long long>> condInstr = condition->generateInstructions();
  
    switch (condition->getConditionType())
    {
        case 1:
            //Eq
            for(int i = 0; i < condInstr.size();i++){
                if (condInstr[i].first == "JPOS " && condInstr[i].second != 0){
                    cout << "if" << endl;
                    condInstr[i].second = commands.size() - i;
                }
                else if(condInstr[i].first == "JPOS " && condInstr[i].second == 0){  
                    cout << "else" << endl;
                    cout << i << endl;
                    condInstr[i].second = -(commands.size() + (i));
                }
            }
            commands.insert(commands.end(), condInstr.begin(), condInstr.end());
            break;
        case 2:
            //Neq
            for (int i = 0; i < condInstr.size(); i++){
                if(condInstr[i].first == "JZERO "){
                    condInstr[i].second = -(commands.size() + i);
                } else if(condInstr[i].first == "JPOS "){
                    condInstr[i].second = commands.size() - i;
                }
            }
      
            commands.insert(commands.end(), condInstr.begin(), condInstr.end());
            break;
        case 3: case 4:
            //Gt or Lt
            for(int i = 0; i < condInstr.size(); i++){
                if((condInstr[i].first == "JZERO " || condInstr[i].first == "JPOS ") && condInstr[i].second == 0){
                    condInstr[i].second = -(commands.size() + i);
                }
            }

            commands.insert(commands.end(), condInstr.begin(), condInstr.end());
            break;
        case 5: case 6:
            //Geq or Leq
            for(int i = 0; i < condInstr.size(); i++){
                if(condInstr[i].first == "JPOS "){
                    condInstr[i].second = -(commands.size() + i);
                }
            }
            commands.insert(commands.end(), condInstr.begin(), condInstr.end());
            break;
    }
    return commands;
}

While::While(Condition *condition, Commands *commands, Variable *var,string procName){
    this->condition = condition;
    this->commands = commands->getInstructions();

    checkLoopVariables(procName,commands, var);
}

While::~While(){
    delete condition;
}

vector<pair<string,long long>> While::generateInstructions(){
    vector<pair<string,long long>> condInstr = condition->generateInstructions();
    
   
    for(int i = 0; i < condInstr.size();i++){
        if((condInstr[i].first == "JZERO " || condInstr[i].first == "JPOS " || condInstr[i].first == "JUMP ")&&condInstr[i].second == 0){
            condInstr[i].second = (condInstr.size()-i) + commands.size() + 1;
        }
    }

    condInstr.insert(condInstr.end(), commands.begin(), commands.end());
    condInstr.push_back(make_pair("JUMP ", -(condInstr.size())));

    return condInstr;
}

/* Assign::Assign(string identifier, Expression *expression, Variable *var){
    this->identifier = identifier;
    this->expression = expression;
    this->var = var;
    assignCheckVariable(identifier, var);
} */

Assign::Assign(string identifier, Expression *expression, Variable *var, string procName, int line){
    this->identifier = identifier;
    this->expression = expression;
    this->var = var;
    this->procName = procName;
    this->isProcHead = var->isProcHeadAssigned(procName, identifier);
    assignCheckVariable(procName,identifier, var, expression, line);
}

Assign::~Assign(){
    delete expression;
}

vector<pair<string,long long>> Assign::generateInstructions(){ //change to vector
    
    vector<pair<string,long long>> exprInstr = expression->generateInstructions();

    switch (expression->getExpressionType())
    {
    case 1: case 2:
        //AddExpr or SubExpr
        
        if(isProcHead){
            exprInstr.push_back(make_pair("STOREI ", var->getProcHeadMemoryPosition(procName,identifier)));    
        } else if (procName != "null"){
            exprInstr.push_back(make_pair("STORE ", var->getProcLocalMemoryPosition(procName, identifier)));
        } else{
            exprInstr.push_back(make_pair("STORE ", var->getMemoryPosition(identifier)));
        }
        

        //assemblerLineNo += exprInstr.size();
        //set assemblyline in assebly.cpp
        break;
    
    case 3:     //MulExpr
        /* for(int i = 0; i < exprInstr.size(); i++){
            assemblerLineNo++;
            if (exprInstr[i].first == "JPOS ")
            {
                exprInstr[i].second = assemblerLineNo + exprInstr[i].second;
            }
        } */

        if(isProcHead){
            exprInstr.push_back(make_pair("STOREI ", var->getProcHeadMemoryPosition(procName,identifier)));    
        } else if (procName != "null"){
            exprInstr.push_back(make_pair("STORE ", var->getProcLocalMemoryPosition(procName, identifier)));
        } else{
            exprInstr.push_back(make_pair("STORE ", var->getMemoryPosition(identifier)));
        }
        //assemblerLineNo++;

        /* if (expression->getSetLeftValueToTemp())
        {
            exprInstr.push_back(make_pair("LOAD ",var->getTempValueMemoryPosition()));
            if(expression->getLeft()->isHeadValue()){
                exprInstr.push_back(make_pair("STOREI ", expression->getLeft()->getValue()));
            }else{
                exprInstr.push_back(make_pair("STORE ", expression->getLeft()->getValue()));
            }
            //assemblerLineNo += 2;   
        }
        else if (expression->getSetRightValueToTemp())
        {
            exprInstr.push_back(make_pair("LOAD ",var->getTempValueMemoryPosition()));
            if(expression->getRight()->isHeadValue()){
                exprInstr.push_back(make_pair("STOREI ", expression->getRight()->getValue()));
            }else{
                exprInstr.push_back(make_pair("STORE ",expression->getRight()->getValue()));
            }   
            //assemblerLineNo += 2; 
        } */

        break;
        //set assemblyline in assebly.cpp

    case 4:     //DivExpr
        /* for(int i = 0; i < exprInstr.size(); i++)
        {
            assemblerLineNo++;
            if(exprInstr[i].first == "JPOS " || exprInstr[i].first == "JZERO " || exprInstr[i].first == "JUMP ")
            {
                exprInstr[i].second = assemblerLineNo + exprInstr[i].second;
            }
        } */
        if(isProcHead){
            exprInstr.push_back(make_pair("STOREI ", var->getProcHeadMemoryPosition(procName,identifier)));    
        } else if (procName != "null"){
            exprInstr.push_back(make_pair("STORE ", var->getProcLocalMemoryPosition(procName, identifier)));
        } else{
            exprInstr.push_back(make_pair("STORE ", var->getMemoryPosition(identifier)));
        }
        //assemblerLineNo++;

        /* if (expression->getSetLeftValueToTemp())
        {
            exprInstr.push_back(make_pair("LOAD ",var->getTempValueMemoryPosition()));
            if(expression->getLeft()->isHeadValue()){
                exprInstr.push_back(make_pair("STOREI ", expression->getLeft()->getValue()));
            }else{
                exprInstr.push_back(make_pair("STORE ", expression->getLeft()->getValue()));
            }
            //assemblerLineNo += 2;   
        } */

        break;

    case 5:     //ModExpr
        /* for(int i = 0; i < exprInstr.size(); i++)
        {
            assemblerLineNo++;
            if(exprInstr[i].first == "JPOS " || exprInstr[i].first == "JZERO " || exprInstr[i].first == "JUMP ")
            {
                exprInstr[i].second = assemblerLineNo + exprInstr[i].second;
            }
        } */

        if(isProcHead){
            exprInstr.push_back(make_pair("STOREI ", var->getProcHeadMemoryPosition(procName,identifier)));    
        } else if (procName != "null"){
            exprInstr.push_back(make_pair("STORE ", var->getProcLocalMemoryPosition(procName, identifier)));
        } else{
            exprInstr.push_back(make_pair("STORE ", var->getMemoryPosition(identifier)));
        }
        //assemblerLineNo++;

        /* if (expression->getSetLeftValueToTemp())
        {
            exprInstr.push_back(make_pair("LOAD ",var->getTempValueMemoryPosition()));
            if(expression->getLeft()->isHeadValue()){
                exprInstr.push_back(make_pair("STOREI ", expression->getLeft()->getValue()));
            }else{
                exprInstr.push_back(make_pair("STORE ", expression->getLeft()->getValue()));
            }
            //assemblerLineNo += 2;   
        } */

        break;
    case 6:     //ValueExpr

        if(isProcHead){
            exprInstr.push_back(make_pair("STOREI ", var->getProcHeadMemoryPosition(procName,identifier)));    
        } else if (procName != "null"){
            exprInstr.push_back(make_pair("STORE ", var->getProcLocalMemoryPosition(procName, identifier)));
        } else{
            exprInstr.push_back(make_pair("STORE ", var->getMemoryPosition(identifier)));
        }

        //assemblerLineNo++;
        break;
    default:
        break;
    }
    return exprInstr;
}

/* Read::Read(string identifier, Variable *var){
    this->identifier = identifier;
    this->var = var;
    readCheckVariable(identifier, var);
} */

Read::Read(string identifier, string procName, Variable *var){
    this->identifier = identifier;
    this->procName = procName;
    this->var = var;
    readCheckVariable(procName,var->isProcHeadAssigned(procName, identifier),identifier, var);
}

Read::~Read(){}

vector<pair<string,long long>> Read::generateInstructions(){
    var->assignMemory(identifier);
    if (procName != "null")
    {
        instructions.push_back(make_pair("GET ", var->getProcLocalMemoryPosition(procName, identifier)));
    }else{
        instructions.push_back(make_pair("GET ", var->getMemoryPosition(identifier)));
    }
    //assemblerLineNo++;
    //change assemblyline in assembly.cpp
    return instructions;
}

Write::Write(Value *value, Variable *var, string procName){
    this->value = value;
    this->var = var;
    this->isProcHead = var->isProcHeadAssigned(procName, value->getIdentifier());
    this->procName = procName;
    writeCheckVariable(procName, isProcHead, value->getIdentifier(), var);
}

Write::~Write(){
    delete value;
}

vector<pair<string,long long>> Write::generateInstructions(){
    if(value->isConst()){
        instructions.push_back(make_pair("SET ", value->getValue()));
        instructions.push_back(make_pair("PUT ", 0));
    }else{
        if(value->isHeadValue()){
            instructions.push_back(make_pair("LOADI ", value->getValue()));
        }else{
            instructions.push_back(make_pair("LOAD ", value->getValue()));
        }
        instructions.push_back(make_pair("PUT ", 0));
    }
    return instructions;
}

Call_Proc::Call_Proc(string procName, vector<string> assignedValues, Variable *var, string currentProc){
    this->procName = procName;
    this->assignedValues = assignedValues;
    this->var = var;
    this->pointerValues = var->getProcHeadVariables(procName);
    this->currentProc = currentProc;

    if(assignedValues.size() != pointerValues.size()-2){
        throw "Error: Number of arguments passed to procedure " + procName + " is not equal to the number of parameters";
    }
    for(int i = 0; i < assignedValues.size(); i++){
        callCheckVariable(currentProc,assignedValues[i], var);    
    }
    
}

Call_Proc::~Call_Proc(){}

vector<pair<string,long long>> Call_Proc::generateInstructions(){
    for(int i = 0; i < assignedValues.size(); i++){
        if(var->isProcHeadAssigned(currentProc, assignedValues[i])){
            //throw "Error: Cannot pass a procedure head variable as an argument to a procedure";
            instructions.push_back(make_pair("LOAD ", var->getProcHeadMemoryPosition(currentProc, assignedValues[i])));
        }else if(currentProc != "null"){
            instructions.push_back(make_pair("SET ", var->getProcLocalMemoryPosition(currentProc, assignedValues[i])));
        }
        else{
            instructions.push_back(make_pair("SET ", var->getMemoryPosition(assignedValues[i])));
        }
        instructions.push_back(make_pair("STORE ", var->getProcHeadMemoryPosition(procName, pointerValues[i].first)));
    }

    instructions.push_back(make_pair("set ", 0)); //current line + 3

   
    //cout << pointerValues.end()[-2].first << endl;

    instructions.push_back(make_pair("STORE ", var->getProcHeadMemoryPosition(procName, pointerValues[pointerValues.size()-2].first)));//pointerValues.end()[-2].first)));
    instructions.push_back(make_pair("pJUMP ", var->getProcHeadMemoryPosition(procName, pointerValues[pointerValues.size()-1].first)));//pointerValues.end()[-1].first)));
    return instructions;  
}


