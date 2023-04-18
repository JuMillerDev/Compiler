#include "value.hpp"

using namespace std;

Value::~Value(){
}

ConstValue::ConstValue(long long value){
    this->value = value;
}

ConstValue::~ConstValue(){
}

bool ConstValue::isConst(){
    return true;
}

long long ConstValue::getValue(){
    return value;
}

string ConstValue::getIdentifier(){
    return "";
}

string ConstValue::getProcName(){
    return "";
}

bool ConstValue::isHeadValue(){
    return false;
}

bool ConstValue::isLocalValue(){
    return false;
}

/* VarValue::VarValue(string identifier, Variable *var){
    this->var = var;
    this->identifier = identifier;
} */

VarValue::VarValue(string procName, string identifier, Variable *var){
    this->var = var;
    this->identifier = identifier;
    this->procName = procName;
    this->headValue = var->isProcHeadAssigned(procName, identifier);
}

VarValue::~VarValue(){
}

bool VarValue::isConst(){
    return false;
}

string VarValue::getIdentifier(){
    return identifier;
}

long long VarValue::getValue(){
    if (headValue){
        return var->getProcHeadMemoryPosition(procName, identifier);
    }
    else if(procName != "null")
        return var->getProcLocalMemoryPosition(procName, identifier);
    else{
        return var->getMemoryPosition(identifier);
    }
}

string VarValue::getProcName(){
    return procName;
}

bool VarValue::isHeadValue(){
    return headValue;
}

bool VarValue::isLocalValue(){
    if (procName != "null")
        return true;
    else
        return false;
}


