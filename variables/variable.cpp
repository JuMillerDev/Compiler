#include "variable.hpp"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

void Variable::addNotAssignedVariable(string procName,string identifier){

    if(find(procNamesNotAssigned.begin(), procNamesNotAssigned.end(), procName) == procNamesNotAssigned.end()){
        procNamesNotAssigned.push_back(procName);
        not_assigned.push_back(vector<pair<string,long long>>());
    }
    vector<string>::iterator it = find(procNamesNotAssigned.begin(), procNamesNotAssigned.end(), procName);
    int index = distance(procNamesNotAssigned.begin(), it);
    not_assigned[index].push_back(make_pair(identifier, not_assigned_position));


    it = find(procNames.begin(), procNames.end(), procName);
    if(it != procNames.end()){
        int index = distance(procNames.begin(), it);
        for(auto & i : proc_local_variables[index]){
            if(i.first == identifier){
                i.second = not_assigned_position;
                not_assigned_position--;
                return;
            }
        }
    }
    throw ("Error: Variable " + identifier + " is not declared in procedure " + procName);
}

pair<string,string> Variable::getNotAssignedVariable(long long position){
    for(int i = 0; i < not_assigned.size(); i++){
        for(int j = 0; j < not_assigned[i].size(); j++){
            if(not_assigned[i][j].second == position){
                return make_pair(procNamesNotAssigned[i], not_assigned[i][j].first);
            }
        }
    }
    return make_pair("","");
}

void Variable::declareVariable(string identifier){
    main_local_variables.push_back(make_pair(identifier, -1));
}

void Variable::declareProcedure(string identifier){
    procNames.push_back(identifier);
    proc_local_variables.push_back(vector<pair<string, long long>>());
    proc_head_variables.push_back(vector<pair<string, long long>>());
}

void Variable::setWarningMessage(string message){
    this->warningMessage = message;
}

string Variable::getWarningMessage(){
    return warningMessage;
}

void Variable::declareProcLocalVariable(string procName,string identifier){
    vector<string>::iterator it = find(procNames.begin(), procNames.end(), procName);
    if(it != procNames.end()){
        int index = distance(procNames.begin(), it);
        for(auto & i : proc_local_variables[index]){
            if(i.first == identifier){
                throw "Error: Variable " + identifier + " is already declared in procedure " + procName;
            }
        } 
        for(auto & i : proc_head_variables[index]){
            if(i.first == identifier){
                throw "Error: Variable " + identifier + " is already declared in procedure head " + procName;
            }
        }
        proc_local_variables[index].push_back(make_pair(identifier, -1));
    }
    else{
        throw "Error: Procedure " + procName + " is not declared";
    }
}

void Variable::assignProcHeadVariable(string procName,string identifier){
    vector<string>::iterator it = find(procNames.begin(), procNames.end(), procName);
    if(it != procNames.end()){
        int index = distance(procNames.begin(), it);
        for(auto & i : proc_head_variables[index]){
            if(i.first == identifier){
                throw "Error: Variable " + identifier + " is already declared in procedure head " + procName;
            }
        }
        proc_head_variables[index].push_back(make_pair(identifier, memory_position));
        memory_position++;
    }
    else{
        throw "Error: Procedure " + procName + " is not declared";
    }
}

void Variable::assignProcHeadVariables(string procName,vector<string> identifiers){
    for(auto & i : identifiers){
        assignProcHeadVariable(procName, i);
    }
}

//TODO:
//declareProcHeadVariable() have assigned memory at declaration
//declareProcBodyVariable()

bool Variable::isDeclared(string identifier){
    for (auto & i : main_local_variables){
        if(i.first == identifier){
            return true;
        }
    }
    return false;
}

long long Variable::getThirdTempValueMemoryPosition(){
     if(thirdTempValue == -1){
        thirdTempValue = memory_position;
        memory_position++;
    }
    return thirdTempValue;
}

long long Variable::getErrorLine(){
    return errorLine;
}

void Variable::setErrorLine(long long line){
    if(this->errorLine == -1){
        this->errorLine = line;
    }
}

long long Variable::getFourthTempValueMemoryPosition(){
    if(fourthTempValue == -1){
        fourthTempValue = memory_position;
        memory_position++;
    }
    return fourthTempValue;
}

long long Variable::getFifthTempValueMemoryPosition(){
    if(fifthTempValue == -1){
        fifthTempValue = memory_position;
        memory_position++;
    }
    return fifthTempValue;
}

bool Variable::isProcLocalDeclared(string proc, string identifier){
    vector<string>::iterator it = find(procNames.begin(), procNames.end(), proc);
    if(it != procNames.end()){
        int index = distance(procNames.begin(), it);
        vector<pair<string,long long>> temp = proc_local_variables[index];
        for(auto & i : temp){
            if(i.first == identifier){
                return true;
            }
        }
    }
    return false;
}

bool Variable::isProcHeadDeclared(string proc, string identifier){
    vector<string>::iterator it = find(procNames.begin(), procNames.end(), proc);
    if(it != procNames.end()){
        int index = distance(procNames.begin(), it);
        vector<pair<string,long long>> temp = proc_head_variables[index];
        for(auto & i : temp){
            if(i.first == identifier){
                return true;
            }
        }
    }
    return false;
}

bool Variable::isAssigned(string identifier){
    for (auto & i : main_local_variables){
        if(i.first == identifier){
            if(i.second <= -1){
                return false;
            }
            return true;
        }
    }
    return false;
}

bool Variable::isProcDeclared(string proc){
    vector<string>::iterator it = find(procNames.begin(), procNames.end(), proc);
    if(it != procNames.end()){
        return true;
    }
    return false;
}

bool Variable::isProcLocalAssigned(string proc, string identifier){
    vector<string>::iterator it = find(procNames.begin(), procNames.end(), proc);
    if(it != procNames.end()){
        int index = distance(procNames.begin(), it);
        vector<pair<string,long long>> temp = proc_local_variables[index];
        for(auto & i : temp){
            if(i.first == identifier){
                if(i.second <= -1){
                    return false;
                }
                return true;
            }
        }
    }
    return false;
}

//also parse line number to print error
void Variable::assignMemory(string identifier){
    for(auto & i : main_local_variables){
        if(i.first == identifier){
            i.second = memory_position;
            memory_position++;
            return;
        }
    }
    throw "Error: Variable " + identifier + " is not declared";
}

void Variable::assignProcLocalMemory(string proc, string identifier){
    vector<string>::iterator it = find(procNames.begin(), procNames.end(), proc);
    if(it != procNames.end()){
        int index = distance(procNames.begin(), it);
        for(auto & i : proc_local_variables[index]){
            if(i.first == identifier){
                i.second = memory_position;
                memory_position++;
                return;
            }
        }
    }
}

void Variable::assignProcHeadStart(string proc, long long line){
    vector<string>::iterator it = find(procNames.begin(), procNames.end(), proc);
    if(it != procNames.end()){
        int index = distance(procNames.begin(), it);
        proc_head_variables[index].push_back(make_pair("start", line));
    }
}

long long Variable::getMemoryPosition(string identifier){
    for(auto & i : main_local_variables){
        if(i.first == identifier){
            return i.second;
        }
    }
    return -1;
}

bool Variable::isProcHeadAssigned(string proc, string identifier){
    vector<string>::iterator it = find(procNames.begin(), procNames.end(), proc);
    if(it != procNames.end()){
        int index = distance(procNames.begin(), it);
        for(auto & i : proc_head_variables[index]){
            if(i.first == identifier){
                if(i.second <= -1){
                    return false;
                }
                return true;
            }
        }
    }
    return false;
}

vector<pair<string,long long>> Variable::getProcHeadVariables(string proc){
    vector<string>::iterator it = find(procNames.begin(), procNames.end(), proc);
    if(it != procNames.end()){
        int index = distance(procNames.begin(), it);
        return proc_head_variables[index];
    }
    if(!isProcDeclared(proc)){
        throw "Error: Procedure " + proc + " is not declared";
    }
    return vector<pair<string,long long>>();
}

long long Variable::getProcLocalMemoryPosition(string proc,string identifier){

    vector<string>::iterator it = find(procNames.begin(), procNames.end(), proc);
    if(it != procNames.end()){
        int index = distance(procNames.begin(), it);
        for(auto & i : proc_local_variables[index]){
            if(i.first == identifier){
                return i.second;
            }
        }
    }
    return -1;
}

long long Variable::getProcHeadMemoryPosition(string proc,string identifier){
    vector<string>::iterator it = find(procNames.begin(), procNames.end(), proc);
    if(it != procNames.end()){
        int index = distance(procNames.begin(), it);
        for(auto & i : proc_head_variables[index]){
            if(i.first == identifier){
                return i.second;
            }
        }
    }
    return -1;
}

void Variable::printVariables(){
    for(auto & i : main_local_variables){
        cout << i.first << " " << i.second << endl;
    }
}

long long Variable::getTempValueMemoryPosition(){
    if(tempValue == -1){
        tempValue = memory_position;
        memory_position++;
    }
    return tempValue;
}

long long Variable::getIteratorValueMemoryPosition(){
    if(iteratorValue == -1){
        iteratorValue = memory_position;
        memory_position++;
    }
    return iteratorValue;
}

bool Variable::isOneValueAssigned(){
    if(oneValue == -1){
        return false;
    }
    return true;
}

long long Variable::getOneValueMemoryPosition(){
    if(oneValue == -1){
        oneValue = memory_position;
        memory_position++;
    }
    return oneValue;
}

long long Variable::getSecondTempValueMemoryPosition(){
    if(secondTempValue == -1){
        secondTempValue = memory_position;
        memory_position++;
    }
    return secondTempValue;
}