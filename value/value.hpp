#pragma once

#include <string>

using namespace std;

#include "../variables/variable.hpp"


/*
Value: 
    | num -> ConstValue
    | var -> VarValue
*/

class Value{
public:
    virtual ~Value() = 0;
    virtual bool isConst() = 0;
    virtual long long getValue() = 0;
    virtual string getIdentifier() = 0;
    virtual string getProcName() = 0;
    virtual bool isHeadValue() = 0;
    virtual bool isLocalValue() = 0;
};


class ConstValue : public Value{
private:
    long long value;
public:
    ConstValue(long long value);
    ~ConstValue();
    bool isConst() override;
    long long getValue() override;
    string getIdentifier() override; //return empty string
    string getProcName() override; //return empty string
    bool isHeadValue() override; //return false
    bool isLocalValue() override; //return false
};

class VarValue : public Value{
private:
    Variable *var;
    string identifier;
    string procName;
    bool headValue;
public:
    //VarValue(string identifier, Variable *var);
    VarValue(string procName,string identifier, Variable *var);
    ~VarValue();
    bool isConst() override;
    string getIdentifier() override;
    long long getValue() override; // memory position
    string getProcName() override;
    bool isHeadValue() override;
    bool isLocalValue() override;
};