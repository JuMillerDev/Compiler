//control flow for assembly lines
//method that sets ju,p to main if there are procedures
//write assembly code to output file
#include <string>
#include <vector>
#include "../variables/variable.hpp"

using namespace std;


class Assembly{
    private:
    long long assemblyLineNo = -1;
    bool areProceduresPresent = false;
    string fileName;
    vector<pair<string,long long>> instructions;
    public:
    Assembly();
    Assembly(string fileName);
    ~Assembly();
    long long getAssemblyLineNo();
    void writeToFile();
    void addProcCode(vector<pair<string,long long>> proc_instructions, Variable *var, string procName);
    void finishCode(vector<pair<string,long long>> main_instructions, Variable *var);
    void setAssemblyLineNo(long long AssemblyLineNo);
    void addMain();
    void setAreProceduresPresent(bool areProceduresPresent);
    void checkForNotAssignedVariables(Variable *var);
    void checkForNotAssignedVariablesProc(Variable *var, string procName);
};