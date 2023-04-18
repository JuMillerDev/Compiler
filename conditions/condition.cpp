#include "condition.hpp"
using namespace std;

Condition::~Condition(){}

void Condition::checkVariables(Value *left, Value *right){
    //if value is a variable, get its memory position
    long long leftValue = left->getValue();
    long long rightValue = right->getValue();

    if (leftValue == -1)
    {
        //cout << "Error: Variable " << left->getIdentifier() << " is not assigned" << endl;
        throw ("Error: Variable " + left->getIdentifier() + " is not assigned");
    } else if (rightValue == -1)
    {
        //cout << "Error: Variable " << right->getIdentifier() << "is not assigned" << endl;
        throw ("Error: Variable " + right->getIdentifier() +" is not assigned");  
    }  
}

Eq::Eq(Value *left, Value *right, long long tempMemoryPosition){
    this->left = left;
    this->right = right;
    this->tempMemoryPosition = tempMemoryPosition; 
    checkVariables(left, right);
}

Eq::~Eq(){
    delete left;
    delete right;
}

vector<pair<string,long long>> Eq::generateInstructions(){
    bool leftIsConst = left->isConst();
    bool rightIsConst = right->isConst();

    long long leftValue = left->getValue();
    long long rightValue = right->getValue();

    if(leftIsConst && rightIsConst){
        if(leftValue != rightValue){
            instructions.push_back(make_pair("JUMP ", 0)); //if jump value is 0, the value inserted must be after one block of instructions
        } 
    }
     else if (leftIsConst && !rightIsConst)
    {
        if (leftValue == 0)
        {
            if(right->isHeadValue()){
                instructions.push_back(make_pair("LOADI ",rightValue));
            } else{
                instructions.push_back(make_pair("LOAD ",rightValue));
            }
            instructions.push_back(make_pair("JPOS ", 0));
        }
        else
        {
            instructions.push_back(make_pair("SET ",leftValue));
            instructions.push_back(make_pair("STORE ",tempMemoryPosition));
            if(right->isHeadValue()){
                instructions.push_back(make_pair("LOADI ",rightValue));
            } else{
                instructions.push_back(make_pair("LOAD ",rightValue));
            }
            instructions.push_back(make_pair("SUB " ,tempMemoryPosition));
            instructions.push_back(make_pair("JPOS ",0));

            instructions.push_back(make_pair("LOAD ",tempMemoryPosition));
            if(right->isHeadValue()){
                instructions.push_back(make_pair("SUBI ",rightValue));
            } else{
                instructions.push_back(make_pair("SUB ",rightValue));
            }
            instructions.push_back(make_pair("JPOS ",0));
        }
    }
    else if (!leftIsConst && rightIsConst)  
    {
        if (rightValue == 0){
            if(left->isHeadValue()){
                instructions.push_back(make_pair("LOADI ",leftValue));
            } else{
                instructions.push_back(make_pair("LOAD ",leftValue));
            }
            instructions.push_back(make_pair("JPOS ",0));
        }
        else {

            instructions.push_back(make_pair("SET " ,rightValue));
            instructions.push_back(make_pair("STORE " ,tempMemoryPosition));
            if(left->isHeadValue()){
                instructions.push_back(make_pair("LOADI " ,leftValue));
            } else{
                instructions.push_back(make_pair("LOAD " ,leftValue));
            }
            instructions.push_back(make_pair("SUB " ,tempMemoryPosition));
            instructions.push_back(make_pair("JPOS ",0));
            instructions.push_back(make_pair("LOAD " ,tempMemoryPosition));
            if(left->isHeadValue()){
                instructions.push_back(make_pair("SUBI " ,leftValue));
            } else{
                instructions.push_back(make_pair("SUB " ,leftValue));
            }
            instructions.push_back(make_pair("JPOS ",0));  //jump to false
        }
    } else{
        if(left->isHeadValue()){
            instructions.push_back(make_pair("LOADI " ,leftValue));
        } else{
            instructions.push_back(make_pair("LOAD " ,leftValue));
        }
        if(right->isHeadValue()){
            instructions.push_back(make_pair("SUBI " ,rightValue));
        } else{
            instructions.push_back(make_pair("SUB " ,rightValue));
        }
        instructions.push_back(make_pair("JPOS ",0));

        if(right->isHeadValue()){
            instructions.push_back(make_pair("LOADI " ,rightValue));
        } else{
            instructions.push_back(make_pair("LOAD " ,rightValue));
        }
        if(left->isHeadValue()){
            instructions.push_back(make_pair("SUBI " ,leftValue));
        } else{
            instructions.push_back(make_pair("SUB " ,leftValue));
        }
        instructions.push_back(make_pair("JPOS ",0));
    } 
    return instructions;
}

int Eq::getConditionType(){
    return 1;
}


Neq::Neq(Value *left, Value *right, long long tempMemoryPosition){
    this->left = left;
    this->right = right;
    this->tempMemoryPosition = tempMemoryPosition;
    checkVariables(left, right);
}

Neq::~Neq(){}

vector<pair<string,long long>> Neq::generateInstructions(){
    bool leftIsConst = left->isConst();
    bool rightIsConst = right->isConst();

    long long leftValue = left->getValue();
    long long rightValue = right->getValue();
    if(leftIsConst && rightIsConst){
        if(leftValue == rightValue){
            instructions.push_back(make_pair("JUMP ",0));
        }
    }
    else if (leftIsConst && !rightIsConst)
    {
        if (leftValue == 0)
        {
            if(right->isHeadValue()){
                instructions.push_back(make_pair("LOADI ",rightValue));
            } else{
                instructions.push_back(make_pair("LOAD ",rightValue));
            }
            instructions.push_back(make_pair("JZERO ",0));
            //later on check if the last command is JUMP and if so, cut it as separate element
        }
        else
        {
            instructions.push_back(make_pair("SET " ,leftValue));
            instructions.push_back(make_pair("STORE " ,tempMemoryPosition));
            if(right->isHeadValue()){
                instructions.push_back(make_pair("LOADI " ,rightValue));
            } else{
                instructions.push_back(make_pair("LOAD " ,rightValue));
            }
            instructions.push_back(make_pair("SUB " ,tempMemoryPosition));
            instructions.push_back(make_pair("JPOS ",4));

            instructions.push_back(make_pair("LOAD " ,tempMemoryPosition));
            if(right->isHeadValue()){
                instructions.push_back(make_pair("SUBI " ,rightValue));
            } else{
                instructions.push_back(make_pair("SUB " ,rightValue));
            }
            //jump to second condition commands
            instructions.push_back(make_pair("JZERO ",0));
        }
    }
    else if (!leftIsConst && rightIsConst)  
    {
        if (rightValue == 0){
            if(left->isHeadValue()){
                instructions.push_back(make_pair("LOADI ",leftValue));
            } else{
                instructions.push_back(make_pair("LOAD ",leftValue));
            }
            instructions.push_back(make_pair("JZERO ",0));   //jump +1
        }else {

            instructions.push_back(make_pair("SET " ,rightValue));
            instructions.push_back(make_pair("STORE " ,tempMemoryPosition));
            if(left->isHeadValue()){
                instructions.push_back(make_pair("LOADI " ,leftValue));
            } else{
                instructions.push_back(make_pair("LOAD " ,leftValue));
            }
            instructions.push_back(make_pair("SUB " ,tempMemoryPosition));
            //jump to first condition commands
            instructions.push_back(make_pair("JPOS ",4));

            instructions.push_back(make_pair("LOAD " ,tempMemoryPosition));
            if(left->isHeadValue()){
                instructions.push_back(make_pair("SUBI " ,leftValue));
            } else{
                instructions.push_back(make_pair("SUB " ,leftValue));
            }
            instructions.push_back(make_pair("JZERO ",0));
        }
    } else if (leftIsConst && !rightIsConst)
    {
        if(leftValue == 0){
            if(right->isHeadValue()){
                instructions.push_back(make_pair("LOADI ",rightValue));
            } else{
                instructions.push_back(make_pair("LOAD ",rightValue));
            }
            instructions.push_back(make_pair("JZERO ",0));
        }else{

            instructions.push_back(make_pair("SET " ,leftValue));
            instructions.push_back(make_pair("STORE " ,tempMemoryPosition));
            if(right->isHeadValue()){
                instructions.push_back(make_pair("LOADI " ,rightValue));
            } else{
                instructions.push_back(make_pair("LOAD " ,rightValue));
            }
            instructions.push_back(make_pair("SUB " ,tempMemoryPosition));
            instructions.push_back(make_pair("JPOS ",4));

            instructions.push_back(make_pair("LOAD " ,tempMemoryPosition));
            if(right->isHeadValue()){
                instructions.push_back(make_pair("SUBI " ,rightValue));
            } else{
                instructions.push_back(make_pair("SUB " ,rightValue));
            }
            instructions.push_back(make_pair("JZERO ",0));  //jump to true //jump to false
        }
    }
    else
    {
        if(left->isHeadValue()){
            instructions.push_back(make_pair("LOADI " ,leftValue));
        } else{
            instructions.push_back(make_pair("LOAD " ,leftValue));
        }
        if(right->isHeadValue()){
            instructions.push_back(make_pair("SUBI " ,rightValue));
        } else{
            instructions.push_back(make_pair("SUB " ,rightValue));
        }
        instructions.push_back(make_pair("JPOS ",4));   //true condition

        if(right->isHeadValue()){
            instructions.push_back(make_pair("LOADI " ,rightValue));
        } else{
            instructions.push_back(make_pair("LOAD " ,rightValue));
        }
        if(left->isHeadValue()){
            instructions.push_back(make_pair("SUBI " ,leftValue));
        } else{
            instructions.push_back(make_pair("SUB " ,leftValue));
        }
        instructions.push_back(make_pair("JZERO ",0));
    }
    return instructions;
}

int Neq::getConditionType(){
    return 2;
}

Gt::Gt(Value *left, Value *right){
    this->left = left;
    this->right = right;
    checkVariables(left, right);
}

Gt::~Gt(){}

//GT: left > right
vector<pair<string,long long>> Gt::generateInstructions(){
    bool leftIsConst = left->isConst();
    bool rightIsConst = right->isConst();

    long long leftValue = left->getValue();
    long long rightValue = right->getValue();
    if(leftIsConst && rightIsConst){
        if (leftValue < rightValue){
            instructions.push_back(make_pair("JUMP ",0));
        }
    }
    else if (leftIsConst && !rightIsConst)
    {
        instructions.push_back(make_pair("SET " ,leftValue));
        if(right->isHeadValue()){
            instructions.push_back(make_pair("SUBI " ,rightValue));
        } else{
            instructions.push_back(make_pair("SUB " ,rightValue));
        }
        instructions.push_back(make_pair("JZERO ",0));
    }
    else if (!leftIsConst && rightIsConst)
    {
        if(rightValue == 0){
            if(left->isHeadValue()){
                instructions.push_back(make_pair("LOADI ",leftValue));
            } else{
                instructions.push_back(make_pair("LOAD ",leftValue));
            }
            instructions.push_back(make_pair("JZERO ",0));
        }
        instructions.push_back(make_pair("SET " ,rightValue));
        if(left->isHeadValue()){
            instructions.push_back(make_pair("SUBI " ,leftValue));
        } else{
            instructions.push_back(make_pair("SUB " ,leftValue));
        }
        instructions.push_back(make_pair("JPOS ",0));
    }
    else
    {
        if(left->isHeadValue()){
            instructions.push_back(make_pair("LOADI " ,leftValue));
        } else{
            instructions.push_back(make_pair("LOAD " ,leftValue));
        }
        if(right->isHeadValue()){
            instructions.push_back(make_pair("SUBI " ,rightValue));
        } else{
            instructions.push_back(make_pair("SUB " ,rightValue));
        }
        instructions.push_back(make_pair("JZERO ",0));
    }
    return instructions;
}

int Gt::getConditionType(){
    return 3;
}

Lt::Lt(Value *left, Value *right, Variable *var){
    this->left = left;
    this->right = right;
    this->var = var;
    checkVariables(left, right);
}

Lt::~Lt(){}

//Lt: left < right
//write the instructions for the LT command in vector instructions
vector<pair<string,long long>> Lt::generateInstructions(){
    bool leftIsConst = left->isConst();
    bool rightIsConst = right->isConst();

    long long leftValue = left->getValue();
    long long rightValue = right->getValue();

    if(leftIsConst && rightIsConst){
        if(leftValue > rightValue){
            instructions.push_back(make_pair("JUMP ",0));
        }
    }else if (!leftIsConst && rightIsConst)
    {
        instructions.push_back(make_pair("SET ",rightValue));
        instructions.push_back(make_pair("STORE ",var->getTempValueMemoryPosition()));
        if(left->isHeadValue()){
            instructions.push_back(make_pair("LOADI " ,leftValue));
        } else{
            instructions.push_back(make_pair("LOAD " ,leftValue));
        }
        instructions.push_back(make_pair("SUB ",var->getTempValueMemoryPosition()));
        instructions.push_back(make_pair("JPOS ",0));
        instructions.push_back(make_pair("LOAD ",var->getTempValueMemoryPosition()));
        if(left->isHeadValue()){
            instructions.push_back(make_pair("SUBI " ,leftValue));
        } else{
            instructions.push_back(make_pair("SUB " ,leftValue));
        }
        instructions.push_back(make_pair("JZERO ",0));
    }else if (leftIsConst && !rightIsConst)
    {
        instructions.push_back(make_pair("SET " ,leftValue));
        instructions.push_back(make_pair("STORE ",var->getTempValueMemoryPosition()));
        if(right->isHeadValue()){
            instructions.push_back(make_pair("SUBI " ,rightValue));
        } else{
            instructions.push_back(make_pair("SUB " ,rightValue));
        }
        instructions.push_back(make_pair("JPOS ",0));
        if(right->isHeadValue()){
            instructions.push_back(make_pair("LOADI " ,rightValue));
        } else{
            instructions.push_back(make_pair("LOAD " ,rightValue));
        }
        instructions.push_back(make_pair("SUB ",var->getTempValueMemoryPosition()));
        instructions.push_back(make_pair("JZERO ",0));


    } else if (!leftIsConst && !rightIsConst)
    {
        if(left->isHeadValue()){
            instructions.push_back(make_pair("LOADI " ,leftValue));
        } else{
            instructions.push_back(make_pair("LOAD " ,leftValue));
        }
        if(right->isHeadValue()){
            instructions.push_back(make_pair("SUBI " ,rightValue));
        } else{
            instructions.push_back(make_pair("SUB " ,rightValue));
        }
        instructions.push_back(make_pair("JPOS ",0));
        if(right->isHeadValue()){
            instructions.push_back(make_pair("LOADI " ,rightValue));
        } else{
            instructions.push_back(make_pair("LOAD " ,rightValue));
        }
        if(left->isHeadValue()){
            instructions.push_back(make_pair("SUBI " ,leftValue));
        } else{
            instructions.push_back(make_pair("SUB " ,leftValue));
        }
        instructions.push_back(make_pair("JZERO ",0));

    }
    return instructions;
}

int Lt::getConditionType(){
    return 4;
}

Geq::Geq(Value *left, Value *right, long long tempMemoryPosition){
    this->left = left;
    this->right = right;
    this->tempMemoryPosition = tempMemoryPosition;
    checkVariables(left, right);
}

Geq::~Geq(){}

//GTE: left >= right
vector<pair<string,long long>> Geq::generateInstructions(){
    bool leftIsConst = left->isConst();
    bool rightIsConst = right->isConst();

    long long leftValue = left->getValue();
    long long rightValue = right->getValue();
    if(leftIsConst && rightIsConst){
        if(leftValue < rightValue){
            instructions.push_back(make_pair("JUMP ",0));
        }
    }
    else if (!leftIsConst && rightIsConst)
    {
        instructions.push_back(make_pair("SET " ,rightValue));
        if(left->isHeadValue()){
            instructions.push_back(make_pair("SUBI " ,leftValue));
        } else{
            instructions.push_back(make_pair("SUB " ,leftValue));
        }
        instructions.push_back(make_pair("JPOS ",0));
    }
    else if (leftIsConst && !rightIsConst)
    {
        instructions.push_back(make_pair("SET " ,leftValue));
        instructions.push_back(make_pair("STORE " ,tempMemoryPosition));
        if(right->isHeadValue()){
            instructions.push_back(make_pair("LOADI " ,rightValue));
        } else{
            instructions.push_back(make_pair("LOAD " ,rightValue));
        }
        instructions.push_back(make_pair("SUB " ,tempMemoryPosition));
        instructions.push_back(make_pair("JPOS ",0));
    }
    else 
    {
        if(right->isHeadValue()){
            instructions.push_back(make_pair("LOADI " ,rightValue));
        } else{
            instructions.push_back(make_pair("LOAD " ,rightValue));
        }
        if(left->isHeadValue()){
            instructions.push_back(make_pair("SUBI " ,leftValue));
        } else{
            instructions.push_back(make_pair("SUB " ,leftValue));
        }
        instructions.push_back(make_pair("JPOS ",0));
    }
    return instructions;
}

int Geq::getConditionType(){
    return 5;
}


Leq::Leq(Value *left, Value *right, long long tempMemoryPosition){
    this->left = left;
    this->right = right;
    this->tempMemoryPosition = tempMemoryPosition;
    //checkVariables(left, right);
}

Leq::~Leq(){}

//Lte: left <= right
vector<pair<string,long long>> Leq::generateInstructions(){
    bool leftIsConst = left->isConst();
    bool rightIsConst = right->isConst();

    long long leftValue = left->getValue();
    long long rightValue = right->getValue();

    if(leftIsConst && rightIsConst){
        if(leftValue > rightValue){
            instructions.push_back(make_pair("JUMP ",0));
        }
    }
    else if (!leftIsConst && rightIsConst)
    {
        instructions.push_back(make_pair("SET " ,rightValue));
        instructions.push_back(make_pair("STORE " ,tempMemoryPosition));
        if(left->isHeadValue()){
            instructions.push_back(make_pair("LOADI " ,leftValue));
        } else{
            instructions.push_back(make_pair("LOAD " ,leftValue));
        }
        instructions.push_back(make_pair("SUB " ,tempMemoryPosition));
        instructions.push_back(make_pair("JPOS ",0));
    }
    else if (leftIsConst && !rightIsConst)  //const <= var
    {
        instructions.push_back(make_pair("SET " ,leftValue));
        if(right->isHeadValue()){
            instructions.push_back(make_pair("SUBI " ,rightValue));
        } else{
            instructions.push_back(make_pair("SUB " ,rightValue));
        }
        instructions.push_back(make_pair("JPOS ",0));
    }else
    { 
        if(left->isHeadValue()){
            instructions.push_back(make_pair("LOADI " ,leftValue));
        } else{
            instructions.push_back(make_pair("LOAD " ,leftValue));
        }
        if(right->isHeadValue()){
            instructions.push_back(make_pair("SUBI " ,rightValue));
        } else{
            instructions.push_back(make_pair("SUB " ,rightValue));
        }
        instructions.push_back(make_pair("JPOS ",0));
    }
    return instructions;
}

int Leq::getConditionType(){
    return 6;
}
