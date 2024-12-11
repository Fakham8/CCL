#include <iostream>
#include <map>
#include <string>
#include <stdexcept>

using namespace std;

class SymbolTable {
public:
    // Declare a new variable in the symbol table
    void declareVariable(const string &name, const string &type) {
        if (symbolTable.find(name) != symbolTable.end()) {
            throw runtime_error("Semantic error: Variable '" + name + "' is already declared.");
        }
        symbolTable[name] = type;
    }

    // Get the type of a variable
    string getVariableType(const string &name) {
        if (symbolTable.find(name) == symbolTable.end()) {
            throw runtime_error("Semantic error: Variable '" + name + "' is not declared.");
        }
        return symbolTable[name];
    }

    // Check if a variable is declared
    bool isDeclared(const string &name) const {
        return symbolTable.find(name) != symbolTable.end();
    }
    private:
    // Map to store variable names and their types
    map<string, string> symbolTable;
};

int main() {
    SymbolTable symbolTable;

    try {
        // Declare variables
        symbolTable.declareVariable("x", "int");
        symbolTable.declareVariable("y", "float");

        // Display types of variables
        cout << "Type of 'x': " << symbolTable.getVariableType("x") << endl;
        cout << "Type of 'y': " << symbolTable.getVariableType("y") << endl;

        // Attempt to redeclare a variable
        symbolTable.declareVariable("x", "double");
    } catch (const runtime_error &e) {
        // Handle semantic errors
        cerr << e.what() << endl;
    }
try {
        // Check for an undeclared variable
        cout << "Type of 'z': " << symbolTable.getVariableType("z") << endl;
    } catch (const runtime_error &e) {
        // Handle semantic errors
        cerr << e.what() << endl;
    }

    return 0;
}
