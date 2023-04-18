#include "expression.hpp"

using namespace std;

Expression::~Expression(){
}

void Expression::checkVariables(Value *left, Value *right, Variable *var, int line){

    //if value is a variable, get its memory position
    long long leftValue = left->getValue();
    long long rightValue = right->getValue();

    if (leftValue == -1)
    {
        var->setErrorLine(line);
        var->addNotAssignedVariable(left->getProcName(),left->getIdentifier());
        var->setWarningMessage("Warning: Variable " + left->getIdentifier() + " may be unassigned");
        //cout << "Error: Variable " << left->getIdentifier() << " is not assigned" << endl;
        //throw ("Error: Variable " + left->getIdentifier() + " is not assigned");
    } else if (rightValue == -1)
    {
        var->setErrorLine(line); //finish later
        var->addNotAssignedVariable(right->getProcName(),right->getIdentifier());
        var->setWarningMessage("Warning: Variable " + right->getIdentifier() + " may be unassigned");
        //cout << "Error: Variable " << right->getIdentifier() << " is not assigned" << endl;
        //throw ("Error: Variable " + right->getIdentifier() +" is not assigned");
    }   
}

AddExpr::AddExpr(Value *left, Value *right, Variable *var,int line){
    this->left = left;
    this->right = right;
    checkVariables(left,right, var,line);
}

AddExpr::~AddExpr(){
    delete left;
    delete right;
}

Value *AddExpr::getLeft(){
    return left;
}

Value *AddExpr::getRight(){
    return right;
}

int AddExpr::getExpressionType(){
    return 1;
}

vector<pair<string, long long>> AddExpr::generateInstructions(){
    bool leftIsConst = left->isConst();
    bool rightIsConst = right->isConst();

    //if value is a variable, get its memory position
    long long leftValue = left->getValue();
    long long rightValue = right->getValue();

    if (leftIsConst && !rightIsConst)
    {
        //constant and variable or variable and variable
        instructions.push_back(make_pair("SET ",leftValue)); 
        if(right->isHeadValue()){
            instructions.push_back(make_pair("ADDI ",rightValue));
        } else{
            instructions.push_back(make_pair("ADD ",rightValue));
        }
    }
    else if (!leftIsConst && rightIsConst)
    {
        //variable and constant
        instructions.push_back(make_pair("SET ",rightValue));
        if(left->isHeadValue()){
            instructions.push_back(make_pair("ADDI ",leftValue));
        } else{
            instructions.push_back(make_pair("ADD ",leftValue));
        }
 
    } else if (!leftIsConst && !rightIsConst)
    {
        //variable and variable
        if(left->isHeadValue()){
            instructions.push_back(make_pair("LOADI ",leftValue));
        } else{
            instructions.push_back(make_pair("LOAD ",leftValue));
        }
        if(right->isHeadValue()){
            instructions.push_back(make_pair("ADDI ",rightValue));
        } else{
            instructions.push_back(make_pair("ADD ",rightValue));
        }
        
    } else if(leftIsConst && rightIsConst) {
        //constant and constant
        instructions.push_back(make_pair("SET ",(leftValue + rightValue)));
    } else {
        cout << "Error: Wrong instructions given" << endl;
        throw ("Error: Wrong instructions given");
    }

    return instructions;
}

bool AddExpr::getSetLeftValueToTemp(){
    return false;
}

bool AddExpr::getSetRightValueToTemp(){
    return false;
}

SubExpr::SubExpr(Value *left, Value *right, Variable *var,int line){ //also need variable
    this->left = left;
    this->right = right;
    this->tempValue = var->getTempValueMemoryPosition();
    checkVariables(left,right, var, line);
}

SubExpr::~SubExpr(){
    delete left;
    delete right;
}

Value *SubExpr::getLeft(){
    return left;
}

Value *SubExpr::getRight(){
    return right;
}

int SubExpr::getExpressionType(){
    return 2;
}

vector<pair<string,long long>> SubExpr::generateInstructions(){
    bool leftIsConst = left->isConst();
    bool rightIsConst = right->isConst();

    //if value is a variable, get its memory position
    long long leftValue = left->getValue();
    long long rightValue = right->getValue();

    if (leftIsConst && !rightIsConst)
    {
        //constant and variable or variable and variable
        instructions.push_back(make_pair("SET ",leftValue));
        if(right->isHeadValue()){
            instructions.push_back(make_pair("SUBI ",rightValue));
        } else{
            instructions.push_back(make_pair("SUB ",rightValue));
        }
    }
    else if (!leftIsConst && rightIsConst)
    {

        instructions.push_back(make_pair("SET ",rightValue));
        instructions.push_back(make_pair("STORE ",tempValue));
        if(left->isHeadValue()){
            instructions.push_back(make_pair("LOADI ",leftValue));
        } else{
            instructions.push_back(make_pair("LOAD ",leftValue));
        }
        instructions.push_back(make_pair("SUB ",tempValue));
    }
    else if (!leftIsConst && !rightIsConst)
    {
        //variable and variable
        if(left->isHeadValue()){
            instructions.push_back(make_pair("LOADI ",leftValue));
        } else{
            instructions.push_back(make_pair("LOAD ",leftValue));
        }
        if(right->isHeadValue()){
            instructions.push_back(make_pair("SUBI ",rightValue));
        } else{
            instructions.push_back(make_pair("SUB ",rightValue));
        }
        
    } else if (leftIsConst && rightIsConst)
    {
        //constant and constant
        long long counted = leftValue - rightValue;
        if(counted < 0){counted = 0;}
        instructions.push_back(make_pair("SET " ,counted));
    } else {
        cout << "Error: Wrong instructions given" << endl;
        throw ("Error: Wrong instructions given");
    }

    return instructions;
}

bool SubExpr::getSetLeftValueToTemp(){
    return false;
}

bool SubExpr::getSetRightValueToTemp(){
    return false;
}

MulExpr::MulExpr(Value *left, Value *right, Variable *var,int line){ //also need variable
    this->left = left;
    this->right = right;
    this->var = var;
    tempValue = var->getTempValueMemoryPosition();
    iteratorValue = var->getIteratorValueMemoryPosition();
    checkVariables(left,right, var,line);
}

MulExpr::~MulExpr(){
    delete left;
    delete right;
}

Value *MulExpr::getLeft(){
    return left;
}

Value *MulExpr::getRight(){
    return right;
}

int MulExpr::getExpressionType(){
    return 3;
}

vector<pair<string,long long>> MulExpr::generateInstructions(){
    bool leftIsConst = left->isConst();
    bool rightIsConst = right->isConst();

    //if value is a variable, get its memory position
    long long leftValue = left->getValue();
    long long rightValue = right->getValue();

    if (leftIsConst && !rightIsConst)
    {
        if (leftValue == 2)
        {
            if(right->isHeadValue()){
                instructions.push_back(make_pair("LOADI ",rightValue));
            } else{
                instructions.push_back(make_pair("LOAD ",rightValue));
            }
            instructions.push_back(make_pair("ADD ",0));
            return instructions;
        }
        else {

            if(!var->isOneValueAssigned()){
            instructions.push_back(make_pair("SET ",1));
            instructions.push_back(make_pair("STORE ",var->getOneValueMemoryPosition()));
            }
            instructions.push_back(make_pair("SET ",leftValue));
            instructions.push_back(make_pair("STORE ",var->getTempValueMemoryPosition()));
            if(right->isHeadValue()){
                instructions.push_back(make_pair("LOADI ",rightValue));
            } else{
                instructions.push_back(make_pair("LOAD ",rightValue));
            }
            instructions.push_back(make_pair("STORE ",var->getSecondTempValueMemoryPosition()));


            
        }       
    }
    else if (!leftIsConst && rightIsConst)
    {
        if(rightValue == 2){
            if(left->isHeadValue()){
                instructions.push_back(make_pair("LOADI ",leftValue));
            } else{
                instructions.push_back(make_pair("LOAD ",leftValue));
            }
            instructions.push_back(make_pair("ADD ",0));
            return instructions;
        }
        else {
            if(!var->isOneValueAssigned()){
            instructions.push_back(make_pair("SET ",1));
            instructions.push_back(make_pair("STORE ",var->getOneValueMemoryPosition()));
            }
            if(left->isHeadValue()){
                instructions.push_back(make_pair("LOADI ",leftValue));
            } else{
                instructions.push_back(make_pair("LOAD ",leftValue));
            }
            instructions.push_back(make_pair("STORE ",var->getTempValueMemoryPosition()));
            instructions.push_back(make_pair("SET ",rightValue));
            instructions.push_back(make_pair("STORE ",var->getSecondTempValueMemoryPosition()));
        }
    }
    else if (leftIsConst && rightIsConst)
    {
        instructions.push_back(make_pair("SET " ,(leftValue * rightValue)));
        return instructions;
    } else {
        if(!var->isOneValueAssigned()){
            instructions.push_back(make_pair("SET ",1));
            instructions.push_back(make_pair("STORE ",var->getOneValueMemoryPosition()));
        }
        if(left->isHeadValue()){
            instructions.push_back(make_pair("LOADI ",leftValue));
        } else{
            instructions.push_back(make_pair("LOAD ",leftValue));
        }
        instructions.push_back(make_pair("STORE ",var->getTempValueMemoryPosition()));
        if(right->isHeadValue()){
            instructions.push_back(make_pair("LOADI ",rightValue));
        } else{
            instructions.push_back(make_pair("LOAD ",rightValue));
        }
        instructions.push_back(make_pair("STORE ",var->getSecondTempValueMemoryPosition()));

    }

    instructions.push_back(make_pair("SET ", 0));
    instructions.push_back(make_pair("STORE ",var->getIteratorValueMemoryPosition()));
    instructions.push_back(make_pair("LOAD ",var->getSecondTempValueMemoryPosition()));
    instructions.push_back(make_pair("JZERO ", 24)); //change
    instructions.push_back(make_pair("SUB ", var->getOneValueMemoryPosition()));
    instructions.push_back(make_pair("JZERO ", 19)); //change
    instructions.push_back(make_pair("LOAD ",var->getSecondTempValueMemoryPosition()));
    instructions.push_back(make_pair("HALF", 0));
    instructions.push_back(make_pair("STORE ",var->getThirdTempValueMemoryPosition()));
    instructions.push_back(make_pair("ADD ",0));
    instructions.push_back(make_pair("STORE ",var->getThirdTempValueMemoryPosition()));
    instructions.push_back(make_pair("LOAD ",var->getSecondTempValueMemoryPosition()));
    instructions.push_back(make_pair("SUB ",var->getThirdTempValueMemoryPosition()));
    instructions.push_back(make_pair("JZERO ", 4)); //change 
    instructions.push_back(make_pair("LOAD ",var->getIteratorValueMemoryPosition()));
    instructions.push_back(make_pair("ADD ",var->getTempValueMemoryPosition()));
    instructions.push_back(make_pair("STORE ",var->getIteratorValueMemoryPosition()));
    instructions.push_back(make_pair("LOAD ",var->getTempValueMemoryPosition()));
    instructions.push_back(make_pair("ADD ",0));
    instructions.push_back(make_pair("STORE ",var->getTempValueMemoryPosition()));
    instructions.push_back(make_pair("LOAD ",var->getSecondTempValueMemoryPosition()));
    instructions.push_back(make_pair("HALF", 0));
    instructions.push_back(make_pair("STORE ",var->getSecondTempValueMemoryPosition()));
    instructions.push_back(make_pair("JUMP ", -21)); //change ---
    instructions.push_back(make_pair("LOAD ",var->getIteratorValueMemoryPosition()));
    instructions.push_back(make_pair("ADD ",var->getTempValueMemoryPosition()));
    instructions.push_back(make_pair("STORE ",var->getIteratorValueMemoryPosition()));
    instructions.push_back(make_pair("LOAD ",var->getIteratorValueMemoryPosition()));
    //later when assigning to variable set result to tempValue

    return instructions;  
}

bool MulExpr::getSetLeftValueToTemp(){
    return setLeftValueToTemp;
}

bool MulExpr::getSetRightValueToTemp(){
    return setRightValueToTemp;
}

DivExpr::DivExpr(Value *left, Value *right, Variable *var, int line){ //also need variable
    this->left = left;
    this->right = right;
    this->var = var;

    checkVariables(left,right,var,line);
}

DivExpr::~DivExpr(){
    delete left;
    delete right;
}

Value *DivExpr::getLeft(){
    return left;
}

Value *DivExpr::getRight(){
    return right;
}

int DivExpr::getExpressionType(){
    return 4;
}

vector<pair<string,long long>> DivExpr::generateInstructions(){
    bool leftIsConst = left->isConst();
    bool rightIsConst = right->isConst();

    //if value is a variable, get its memory position
    long long leftValue = left->getValue();
    long long rightValue = right->getValue();

    if(leftIsConst && rightIsConst)
    {
        if (rightValue == 0)
        {
            instructions.push_back(make_pair("SET ",0));
        } else{
            instructions.push_back(make_pair("SET ",(leftValue / rightValue)));
        }
        return instructions;
    }
    else if (leftIsConst && !rightIsConst)
    {
        if(leftValue == 0){
            instructions.push_back(make_pair("SET ",0));
            return instructions;
        }
        else
        {
            if(!var->isOneValueAssigned()){
                instructions.push_back(make_pair("SET ",1));
                instructions.push_back(make_pair("STORE ",var->getOneValueMemoryPosition()));
            }

            if(right->isHeadValue()){
                instructions.push_back(make_pair("LOADI ",rightValue));
            } else{
                instructions.push_back(make_pair("LOAD ",rightValue));
            }
            instructions.push_back(make_pair("JZERO ",42));
            instructions.push_back(make_pair("STORE ",var->getTempValueMemoryPosition()));
            instructions.push_back(make_pair("SET ",leftValue));
            instructions.push_back(make_pair("STORE ",var->getSecondTempValueMemoryPosition()));
            instructions.push_back(make_pair("SET ",0));
            instructions.push_back(make_pair("STORE ",var->getThirdTempValueMemoryPosition()));
            instructions.push_back(make_pair("SET ",1));
            instructions.push_back(make_pair("STORE ",var->getIteratorValueMemoryPosition()));
            instructions.push_back(make_pair("STORE ",var->getFourthTempValueMemoryPosition()));
        }
    }
    else if (!leftIsConst && rightIsConst)
    {
        if(rightValue == 0){
            instructions.push_back(make_pair("SET ",0));
            return instructions;
        }
        else if (rightValue == 2)
        {
            if(left->isHeadValue()){
                instructions.push_back(make_pair("LOADI ",leftValue));
            } else{
                instructions.push_back(make_pair("LOAD ",leftValue));
            }
            instructions.push_back(make_pair("HALF",0));
        }
        else
        {
            if(!var->isOneValueAssigned()){
                instructions.push_back(make_pair("SET ",1));
                instructions.push_back(make_pair("STORE ",var->getOneValueMemoryPosition()));
            }

            instructions.push_back(make_pair("SET ",rightValue));
            instructions.push_back(make_pair("STORE ",var->getTempValueMemoryPosition()));
            if(left->isHeadValue()){
                instructions.push_back(make_pair("LOADI ",leftValue));
            } else{
                instructions.push_back(make_pair("LOAD ",leftValue));
            }
            instructions.push_back(make_pair("JZERO ",40));  //jump 9 lines ahead
            instructions.push_back(make_pair("STORE ",var->getSecondTempValueMemoryPosition()));
            instructions.push_back(make_pair("SET ",0));
            instructions.push_back(make_pair("STORE ",var->getThirdTempValueMemoryPosition()));
            instructions.push_back(make_pair("SET ",1));
            instructions.push_back(make_pair("STORE ",var->getIteratorValueMemoryPosition()));
            instructions.push_back(make_pair("STORE ",var->getFourthTempValueMemoryPosition()));
        }
    }
    else if (!leftIsConst && !rightIsConst)
    {
        if(!var->isOneValueAssigned()){
            instructions.push_back(make_pair("SET ",1));
            instructions.push_back(make_pair("STORE ",var->getOneValueMemoryPosition()));
        }

        if(right->isHeadValue()){
            instructions.push_back(make_pair("LOADI ",rightValue));
        } else{
            instructions.push_back(make_pair("LOAD ",rightValue));
        }
        instructions.push_back(make_pair("JZERO ",43));  //jump 9 lines ahead
        instructions.push_back(make_pair("STORE ",var->getTempValueMemoryPosition()));
        if(left->isHeadValue()){
            instructions.push_back(make_pair("LOADI ",leftValue));
        } else{
            instructions.push_back(make_pair("LOAD ",leftValue));
        }
        instructions.push_back(make_pair("JZERO ",40));  //jump 9 lines ahead
        instructions.push_back(make_pair("STORE ",var->getSecondTempValueMemoryPosition()));
        instructions.push_back(make_pair("SET ",0));
        instructions.push_back(make_pair("STORE ",var->getThirdTempValueMemoryPosition()));
        instructions.push_back(make_pair("SET ",1));
        instructions.push_back(make_pair("STORE ",var->getIteratorValueMemoryPosition()));
        instructions.push_back(make_pair("STORE ",var->getFourthTempValueMemoryPosition())); 
    }

    instructions.push_back(make_pair("LOAD ",var->getTempValueMemoryPosition()));
    instructions.push_back(make_pair("SUB ",var->getSecondTempValueMemoryPosition()));
    instructions.push_back(make_pair("JPOS ",11));  //jump 9 lines ahead
    instructions.push_back(make_pair("LOAD ",var->getTempValueMemoryPosition()));
    instructions.push_back(make_pair("ADD ",0));
    instructions.push_back(make_pair("STORE ",var->getTempValueMemoryPosition()));
    instructions.push_back(make_pair("LOAD ",var->getFourthTempValueMemoryPosition()));
    instructions.push_back(make_pair("ADD ",0));
    instructions.push_back(make_pair("STORE ",var->getFourthTempValueMemoryPosition()));
    instructions.push_back(make_pair("LOAD ",var->getIteratorValueMemoryPosition()));
    instructions.push_back(make_pair("ADD ",var->getOneValueMemoryPosition()));
    instructions.push_back(make_pair("STORE ",var->getIteratorValueMemoryPosition()));
    instructions.push_back(make_pair("JUMP ",-12));  //jump 9 lines back
    instructions.push_back(make_pair("LOAD ",var->getTempValueMemoryPosition()));
    instructions.push_back(make_pair("SUB ",var->getSecondTempValueMemoryPosition()));
    instructions.push_back(make_pair("JPOS ",7));  //jump 9 lines ahead
    instructions.push_back(make_pair("LOAD ",var->getSecondTempValueMemoryPosition()));
    instructions.push_back(make_pair("SUB ",var->getTempValueMemoryPosition()));
    instructions.push_back(make_pair("STORE ",var->getSecondTempValueMemoryPosition()));
    instructions.push_back(make_pair("LOAD ",var->getThirdTempValueMemoryPosition()));
    instructions.push_back(make_pair("ADD ",var->getFourthTempValueMemoryPosition()));
    instructions.push_back(make_pair("STORE ",var->getThirdTempValueMemoryPosition()));
    instructions.push_back(make_pair("LOAD ",var->getTempValueMemoryPosition()));
    instructions.push_back(make_pair("HALF",0));
    instructions.push_back(make_pair("STORE ",var->getTempValueMemoryPosition()));
    instructions.push_back(make_pair("LOAD ",var->getFourthTempValueMemoryPosition()));
    instructions.push_back(make_pair("HALF",0));
    instructions.push_back(make_pair("STORE ",var->getFourthTempValueMemoryPosition()));
    instructions.push_back(make_pair("LOAD ",var->getIteratorValueMemoryPosition()));
    instructions.push_back(make_pair("SUB ",var->getOneValueMemoryPosition()));
    instructions.push_back(make_pair("STORE ",var->getIteratorValueMemoryPosition()));
    instructions.push_back(make_pair("JZERO ",4));  //jump 9 lines back
    instructions.push_back(make_pair("JUMP ",-19));  //jump 9 lines ahead"))
    instructions.push_back(make_pair("SET ",0));
    instructions.push_back(make_pair("STORE ",var->getThirdTempValueMemoryPosition()));
    instructions.push_back(make_pair("LOAD ",var->getThirdTempValueMemoryPosition()));

    return instructions;
}

bool DivExpr::getSetLeftValueToTemp(){
    return setLeftValueToTemp;
}

bool DivExpr::getSetRightValueToTemp(){
    return setRightValueToTemp;
}

ModExpr::ModExpr(Value *left, Value *right, Variable *var,int line){ //also need variable
    this->left = left;
    this->right = right;
    this->var = var;

    checkVariables(left,right,var,line);
}

ModExpr::~ModExpr(){
    delete left;
    delete right;
}

Value *ModExpr::getLeft(){
    return left;
}

Value *ModExpr::getRight(){
    return right;
}

int ModExpr::getExpressionType(){
    return 5;
}

vector<pair<string,long long>> ModExpr::generateInstructions(){
    bool leftIsConst = left->isConst();
    bool rightIsConst = right->isConst();

    long long leftValue = left->getValue();
    long long rightValue = right->getValue();

    if(leftIsConst && rightIsConst){
        if(rightValue == 0){
            instructions.push_back(make_pair("SET ",0));
        }
        else{
            instructions.push_back(make_pair("SET ",leftValue % rightValue));
        }
        return instructions;
    }
    else if (leftIsConst && !rightIsConst)
    {
        if(leftValue == 0){
            instructions.push_back(make_pair("SET ",0));
            return instructions;
        }else{
            if(!var->isOneValueAssigned()){
                instructions.push_back(make_pair("SET ",1));
                instructions.push_back(make_pair("STORE ", var->getOneValueMemoryPosition()));
            }
    
            if(right->isHeadValue()){
                instructions.push_back(make_pair("LOADI ",rightValue));
            } else{
                instructions.push_back(make_pair("LOAD ",rightValue));
            }
            instructions.push_back(make_pair("JZERO ",31));  //jump 16 lines ahead [maybe change in var:var]
            instructions.push_back(make_pair("STORE ", var->getTempValueMemoryPosition())); //b
            instructions.push_back(make_pair("SET ",leftValue));
            instructions.push_back(make_pair("STORE ", var->getSecondTempValueMemoryPosition())); //a
        }
    }
    else if (!leftIsConst && rightIsConst)
    {
        if(rightValue == 0){
            instructions.push_back(make_pair("SET ",0));
            return instructions;
        }
        else{
        
            if(!var->isOneValueAssigned()){
                instructions.push_back(make_pair("SET ",1));
                instructions.push_back(make_pair("STORE ", var->getOneValueMemoryPosition()));
            }

            instructions.push_back(make_pair("SET ", rightValue));
            instructions.push_back(make_pair("STORE ", var->getTempValueMemoryPosition())); //b
            if(left->isHeadValue()){
                instructions.push_back(make_pair("LOADI ",leftValue));
            } else{
                instructions.push_back(make_pair("LOAD ",leftValue));
            }
            instructions.push_back(make_pair("JZERO ",29));  //jump 16 lines ahead [maybe change in var:var]
            instructions.push_back(make_pair("STORE ", var->getSecondTempValueMemoryPosition())); //a
        }
    }
    else
    {

        if(!var->isOneValueAssigned()){
            instructions.push_back(make_pair("SET ",1));
            instructions.push_back(make_pair("STORE ", var->getOneValueMemoryPosition()));
        }

        if(right->isHeadValue()){
            instructions.push_back(make_pair("LOADI ",rightValue));
        } else{
            instructions.push_back(make_pair("LOAD ",rightValue));
        }
        instructions.push_back(make_pair("JZERO ",31));  //jump 16 lines ahead
        instructions.push_back(make_pair("STORE ", var->getTempValueMemoryPosition())); //b
        if(left->isHeadValue()){
            instructions.push_back(make_pair("LOADI ",leftValue));
        } else{
            instructions.push_back(make_pair("LOAD ",leftValue));
        }
        instructions.push_back(make_pair("JZERO ",28));  //jump 16 lines ahead
        instructions.push_back(make_pair("STORE ", var->getSecondTempValueMemoryPosition())); //a
    }

            
    instructions.push_back(make_pair("SET ",1));
    instructions.push_back(make_pair("STORE ", var->getIteratorValueMemoryPosition()));
    instructions.push_back(make_pair("LOAD ", var->getTempValueMemoryPosition()));
    instructions.push_back(make_pair("SUB ", var->getSecondTempValueMemoryPosition()));
    instructions.push_back(make_pair("JPOS ",8));  //jump 16 lines ahead
    instructions.push_back(make_pair("LOAD ", var->getTempValueMemoryPosition()));
    instructions.push_back(make_pair("ADD ", 0));
    instructions.push_back(make_pair("STORE ", var->getTempValueMemoryPosition()));
    instructions.push_back(make_pair("LOAD ", var->getIteratorValueMemoryPosition()));
    instructions.push_back(make_pair("ADD ", var->getOneValueMemoryPosition()));
    instructions.push_back(make_pair("STORE ", var->getIteratorValueMemoryPosition()));
    instructions.push_back(make_pair("JUMP ",-9));  //jump 16 lines ahead
    instructions.push_back(make_pair("LOAD ", var->getTempValueMemoryPosition()));
    instructions.push_back(make_pair("SUB ", var->getSecondTempValueMemoryPosition()));
    instructions.push_back(make_pair("JPOS ",4));  //jump 16 lines ahead
    instructions.push_back(make_pair("LOAD ", var->getSecondTempValueMemoryPosition()));
    instructions.push_back(make_pair("SUB ", var->getTempValueMemoryPosition()));
    instructions.push_back(make_pair("STORE ", var->getSecondTempValueMemoryPosition()));
    instructions.push_back(make_pair("LOAD ", var->getTempValueMemoryPosition()));
    instructions.push_back(make_pair("HALF",0));
    instructions.push_back(make_pair("STORE ", var->getTempValueMemoryPosition()));
    instructions.push_back(make_pair("LOAD ", var->getIteratorValueMemoryPosition()));
    instructions.push_back(make_pair("SUB ", var->getOneValueMemoryPosition()));
    instructions.push_back(make_pair("STORE ", var->getIteratorValueMemoryPosition()));
    instructions.push_back(make_pair("JZERO ",4));  //jump 16 lines ahead
    instructions.push_back(make_pair("JUMP ",-13));  //jump 16 lines ahead
    instructions.push_back(make_pair("SET ",0));
    instructions.push_back(make_pair("STORE ", var->getSecondTempValueMemoryPosition()));
    instructions.push_back(make_pair("LOAD ", var->getSecondTempValueMemoryPosition()));

    return instructions;
}

bool ModExpr::getSetLeftValueToTemp(){
    return false;
}

bool ModExpr::getSetRightValueToTemp(){
    return false;
}

ValueExpr::ValueExpr(Value *value){
    this->value = value;
}

ValueExpr::~ValueExpr(){
    delete value;
}

Value *ValueExpr::getLeft(){
    return value;
}

Value *ValueExpr::getRight(){
    return value;
}

int ValueExpr::getExpressionType(){
    return 6;
}

vector<pair<string,long long>> ValueExpr::generateInstructions(){
    if(value->isConst()){
        instructions.push_back(make_pair("SET ",value->getValue()));
    }
    else{
        if(value->isHeadValue()){
            instructions.push_back(make_pair("LOADI ",value->getValue()));
        } else{
            instructions.push_back(make_pair("LOAD ",value->getValue()));
        }
    }
    return instructions;
}

bool ValueExpr::getSetLeftValueToTemp(){
    return false;
}

bool ValueExpr::getSetRightValueToTemp(){
    return false;
}