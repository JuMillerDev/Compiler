#include "assembly.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

Assembly::Assembly(){}

Assembly::Assembly(string fileName){
    this->fileName = fileName;
}

void Assembly::writeToFile(){
    ofstream file;
    file.open(fileName);
    for(int i = 0; i < instructions.size(); i++){
        if(instructions[i].first == "HALF" || instructions[i].first == "HALT"){
            file << "[" << i << "] "<< instructions[i].first << endl;
        }else{
            file << "[" << i << "] "<< instructions[i].first << instructions[i].second << endl;
        }
    }
    file.close();
} 

void Assembly::addProcCode(vector<pair<string,long long>> proc_instructions, Variable *var, string procName){
    areProceduresPresent = true;
    var->assignProcHeadVariable(procName,"return");
    var->assignProcHeadStart(procName, instructions.size() + 1);
    proc_instructions.push_back(make_pair("JUMPI ", var->getProcHeadMemoryPosition(procName,"return")));

    instructions.insert(instructions.end(), proc_instructions.begin(), proc_instructions.end());

    checkForNotAssignedVariablesProc(var, procName);

    /*   for(int i = 0; i < proc_instructions.size(); i++){
        assemblyLIneNo++;
        if(proc_instructions[i].first == "JZERO " || proc_instructions[i].first == "JUMP " || proc_instructions[i].first == "JPOS "){
          proc_instructions[i].second = assemblyLineNo + proc_instructions[i].second;  
        }
    } */

}

void Assembly::finishCode(vector<pair<string,long long>> main_instructions, Variable *var){

    /* main_instructions.push_back(make_pair("HALT", 0));
    for(int i = 0; i < main_instructions.size(); i++){
        assemblyLineNo++;
        if(main_instructions[i].first == "JZERO " || main_instructions[i].first == "JUMP " || main_instructions[i].first == "JPOS "){
          main_instructions[i].second = assemblyLineNo + main_instructions[i].second;  
        }
    } */
    if(areProceduresPresent){instructions.insert(instructions.begin(),make_pair("mJUMP ", 0));};

    instructions.insert(instructions.end(), main_instructions.begin(), main_instructions.end());
    instructions.push_back(make_pair("HALT", 0));

    checkForNotAssignedVariables(var);

    if(var->getWarningMessage() != ""){
        cout << var->getWarningMessage() << endl;
    }

    for(int i = 0; i < instructions.size(); i++){
        assemblyLineNo++;
        if(instructions[i].first == "JZERO " || instructions[i].first == "JUMP " || instructions[i].first == "JPOS "){
            
            instructions[i].second = assemblyLineNo + instructions[i].second;
            
        } else if (instructions[i].first == "main ")
        {
            instructions[0].first = "JUMP ";
            instructions[0].second = assemblyLineNo;
            instructions.erase(instructions.begin()+assemblyLineNo);

        } else if (instructions[i].first == "set ")
        {
            instructions[i].first = "SET ";
            instructions[i].second = assemblyLineNo + 3;
        } else if (instructions[i].first == "pJUMP ")
        {
            instructions[i].first = "JUMP ";
        }
    }

    writeToFile();
}

void Assembly::checkForNotAssignedVariablesProc(Variable *var, string procName){
    
    for(int i = 0; i < instructions.size(); i++){
        if(!(instructions[i].first == "JZERO " || instructions[i].first == "JUMP " || instructions[i].first == "JPOS ") && var->getNotAssignedVariable(instructions[i].second).second != ""){
            pair<string,string> notAssignedVariable = var->getNotAssignedVariable(instructions[i].second);
            if(var->isProcLocalDeclared(notAssignedVariable.first,notAssignedVariable.second)){
                throw "Error: Variable " + notAssignedVariable.second + " is not assigned";
            }else{
                cout << "here" << endl;
                throw "Error: Variable " + notAssignedVariable.second + " is not declared";
            } 
        }
    }
}

void Assembly::checkForNotAssignedVariables(Variable *var){
    for(int i = 0; i < instructions.size(); i++){
        if(!(instructions[i].first == "JZERO " || instructions[i].first == "JUMP " || instructions[i].first == "JPOS ") && var->getNotAssignedVariable(instructions[i].second).second != ""){
            pair<string,string> notAssignedVariable = var->getNotAssignedVariable(instructions[i].second);
            if(var->isDeclared(notAssignedVariable.second)){
                throw "Error: Variable " + notAssignedVariable.second + " not assigned";
            }else{
                throw "Error: Variable " + notAssignedVariable.second + " is not declared";
            } 
        }
    }
}

void Assembly::addMain(){
    if(areProceduresPresent){
        instructions.push_back(make_pair("main ", 0));
    }
}

long long Assembly::getAssemblyLineNo(){
    return assemblyLineNo;
}

void Assembly::setAssemblyLineNo(long long assemblyLineNo){
    this->assemblyLineNo = assemblyLineNo;
}