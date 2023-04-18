#pragma once

#include <string>
#include <iostream>
#include <ostream>
#include <vector>

#include "../value/value.hpp"
#include "../variables/variable.hpp"

using namespace std;

class Expression{
    public:
        virtual ~Expression() = 0;
        virtual int getExpressionType() = 0;
        virtual Value *getLeft() = 0;
        virtual Value *getRight() = 0;
        virtual vector<pair<string,long long>> generateInstructions() = 0;
        void checkVariables(Value *left, Value *right,Variable *var, int line);
        virtual bool getSetLeftValueToTemp() = 0;
        virtual bool getSetRightValueToTemp() = 0;
};

class AddExpr : public Expression{
    private:
        Value *left;
        Value *right;
        vector<pair<string,long long>> instructions;
    public:
        AddExpr(Value *left, Value *right, Variable *var, int line);
        ~AddExpr();
        Value *getLeft() override;
        Value *getRight() override;
        int getExpressionType() override;
        vector<pair<string,long long>> generateInstructions() override;
        bool getSetLeftValueToTemp() override;
        bool getSetRightValueToTemp() override;
        
};

class SubExpr : public Expression{
    private:
        Value *left;
        Value *right;
        long long tempValue;
        vector<pair<string,long long>> instructions;
    public:
        SubExpr(Value *left, Value *right, Variable *var, int line);
        ~SubExpr();
        Value *getLeft() override;
        Value *getRight() override;
        int getExpressionType() override;
        vector<pair<string, long long>> generateInstructions() override;
        bool getSetLeftValueToTemp() override;
        bool getSetRightValueToTemp() override;
};

class MulExpr : public Expression{
    private:
        Value *left;
        Value *right;
        Variable *var;
        vector<pair<string,long long>> instructions;
        long long tempValue;
        long long iteratorValue;
        long long oneValue;
        bool setLeftValueToTemp = false;
        bool setRightValueToTemp = false;
    public:
        MulExpr(Value *left, Value *right, Variable *var, int line);
        ~MulExpr();
        Value *getLeft() override;
        Value *getRight() override;
        int getExpressionType() override;
        vector<pair<string, long long>> generateInstructions() override;
        bool getSetLeftValueToTemp() override;
        bool getSetRightValueToTemp() override;
};

class DivExpr : public Expression{
    private:
        Value *left;
        Value *right;
        vector<pair<string,long long>> instructions;
        Variable *var;
        bool setLeftValueToTemp = false;
        bool setRightValueToTemp = false;
    public:
        DivExpr(Value *left, Value *right, Variable *var, int line);
        ~DivExpr();
        Value *getLeft() override;
        Value *getRight() override;
        int getExpressionType() override;
        vector<pair<string,long long>> generateInstructions() override;
        bool getSetLeftValueToTemp() override;
        bool getSetRightValueToTemp() override;
};

class ModExpr : public Expression{
    private:
        Value *left;
        Value *right;
        Variable *var;
        vector<pair<string,long long>> instructions;
    public:
        ModExpr(Value *left, Value *right, Variable *var, int line);
        ~ModExpr();
        Value *getLeft() override;
        Value *getRight()  override;
        int getExpressionType() override;
        vector<pair<string,long long>> generateInstructions() override;
        bool getSetLeftValueToTemp() override;
        bool getSetRightValueToTemp() override;
};

class ValueExpr : public Expression{
    private:
        Value *value;
        vector<pair<string,long long>> instructions;
    public:
        ValueExpr(Value *value);
        ~ValueExpr();
        Value *getLeft() override;
        Value *getRight() override;
        int getExpressionType() override;
        vector<pair<string,long long>> generateInstructions() override;
        bool getSetLeftValueToTemp() override;
        bool getSetRightValueToTemp() override;
};
