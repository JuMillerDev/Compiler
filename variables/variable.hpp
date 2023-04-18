#pragma once
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <tuple>

using namespace std;

class Variable{
    private:
        int memory_position = 1; 
        int not_assigned_position = -2;
        vector<pair<string,long long>> main_local_variables;

        vector<string> procNames;
        vector<vector<pair<string, long long>>> proc_local_variables;
        vector<vector<pair<string, long long>>> proc_head_variables;

        vector<vector<pair<string, long long>>> not_assigned;
        vector<string> procNamesNotAssigned;

        string warningMessage = "";

        long long tempValue = -1;
        long long secondTempValue = -1;
        long long thirdTempValue = -1;
        long long fourthTempValue = -1;
        long long fifthTempValue = -1;
        long long iteratorValue = -1;
        long long oneValue = -1;
        long long errorLine = -1;
    public:
        ~Variable(){
            main_local_variables.clear();
        };
        void addNotAssignedVariable(string procName, string identifier);
        void setWarningMessage(string message);
        string getWarningMessage();
        pair<string,string> getNotAssignedVariable(long long mem_pos);
        long long getErrorLine();
        void setErrorLine(long long line);
        void declareVariable(string identifier);
        void declareProcedure(string identifier);
        void declareProcLocalVariable(string procName,string identifier);
        bool isDeclared(string identifier);
        bool isProcDeclared(string proc);
        bool isProcLocalDeclared(string proc, string identifier);
        bool isProcHeadDeclared(string proc, string identifier);
        bool isAssigned(string identifier);
        bool isProcLocalAssigned(string proc, string identifier);
        bool isProcHeadAssigned(string proc, string identifier);
        void assignMemory(string identifier);
        void assignProcLocalMemory(string proc, string identifier);
        void assignProcHeadVariable(string procName,string identifier);
        void assignProcHeadVariables(string procName,vector<string> identifiers);
        void assignProcHeadStart(string procName, long long line);
        vector<pair<string,long long>> getProcHeadVariables(string procName);
        long long getMemoryPosition(string identifier);  
        long long getProcLocalMemoryPosition(string proc,string identifier);
        long long getProcHeadMemoryPosition(string proc,string identifier);
        void printVariables();
        long long getTempValueMemoryPosition();
        long long getSecondTempValueMemoryPosition();
        long long getThirdTempValueMemoryPosition();
        long long getFourthTempValueMemoryPosition();
        long long getFifthTempValueMemoryPosition();
        long long getIteratorValueMemoryPosition();
        bool isOneValueAssigned();
        long long getOneValueMemoryPosition();
};
