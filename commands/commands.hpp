#pragma once
#include <string>
#include <vector>
#include <utility>
#include <ostream>
#include <iostream>

using namespace std;

class Commands{
    private:
        vector<pair<string,long long>> instructions;
    public:
        Commands();
        Commands(vector<pair<string,long long>> instructions);
        ~Commands();
        void addInstruction(string instruction, long long value);
        void addInstructions(vector<pair<string,long long>> instr);
        void printInstructions();
        vector<pair<string,long long>> getInstructions();
        void clearInstructions();
};