#pragma once

#include "../value/value.hpp"

#include <vector>
#include <string>
#include <iostream>
#include <ostream>

using namespace std;


class Condition{
    public:
        virtual ~Condition() = 0;
        virtual vector<pair<string,long long>> generateInstructions() = 0;
        virtual int getConditionType() = 0;
        void checkVariables(Value *left, Value *right);
};

class Eq : public Condition{
private:
    Value *left;
    Value *right;
    vector<pair<string,long long>> instructions;
    long long tempMemoryPosition;
public:
    Eq(Value *left, Value *right ,long long tempMemoryPosition);
    ~Eq();
    vector<pair<string,long long>> generateInstructions() override;
    int getConditionType() override;
};

class Neq : public Condition{
private:
    Value *left;
    Value *right;
    vector<pair<string,long long>> instructions;
    long long tempMemoryPosition;
public:
    Neq(Value *left, Value *right, long long tempMemoryPosition);
    ~Neq();
    vector<pair<string,long long>> generateInstructions() override;
    int getConditionType() override;
};

class Gt : public Condition{
private:
    Value *left;
    Value *right;
    vector<pair<string,long long>> instructions;
public:
    Gt(Value *left, Value *right);
    ~Gt();
    vector<pair<string,long long>> generateInstructions() override;
    int getConditionType() override;
};

class Lt : public Condition{
private:
    Value *left;
    Value *right;
    vector<pair<string,long long>> instructions;
    Variable *var;
public:
    Lt(Value *left, Value *right, Variable *var);
    ~Lt();
    vector<pair<string,long long>> generateInstructions() override;
    int getConditionType() override;
};

class Geq : public Condition{
private:
    Value *left;
    Value *right;
    long long tempMemoryPosition;
    vector<pair<string,long long>> instructions;
public:
    Geq(Value *left, Value *right, long long tempMemoryPosition);
    ~Geq();
    vector<pair<string,long long>> generateInstructions() override;
    int getConditionType() override;
};

class Leq : public Condition{
private:
    Value *left;
    Value *right;
    long long tempMemoryPosition;
    vector<pair<string,long long>> instructions;
public:
    Leq(Value *left, Value *right, long long tempMemoryPosition);
    ~Leq();
    vector<pair<string,long long>> generateInstructions() override;
    int getConditionType() override;
};