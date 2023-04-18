#include "commands.hpp"

using namespace std;

Commands::Commands(){
}

Commands::Commands(vector<pair<string,long long>> instructions){
    this->instructions = instructions;
}

Commands::~Commands(){
}

void Commands::addInstruction(string instruction, long long value){
    instructions.push_back(make_pair(instruction,value));
}

void Commands::addInstructions(vector<pair<string,long long>> instr){
    instructions.insert(instructions.end(), instr.begin(), instr.end());
}

void Commands::printInstructions(){
    for(int i = 0; i < instructions.size(); i++){
        cout << instructions[i].first << " " << instructions[i].second << endl;
    }
}

vector<pair<string,long long>> Commands::getInstructions(){
    return instructions;
}

void Commands::clearInstructions(){
    instructions.clear();
}



