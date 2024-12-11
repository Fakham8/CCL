#include <bits/stdc++.h>
using namespace std;

bool isKeyword(const string& str) {
    return str == "int" || str == "return";  // Add more keywords as needed
}

bool isOperator(char ch) {
    return ch == '=' || ch == '+' || ch == '-' || ch == '*' || ch == '/';
}

bool isSymbol(char ch) {
    return ch == '(' || ch == ')' || ch == '{' || ch == '}' || ch == ';';
}

bool isDigit(char ch) {
    return isdigit(ch);
}

bool isAlpha(char ch) {
    return isalpha(ch) || ch == '_';
}

vector<string> tokenize(const string& code) {
    vector<string> tokens;
    string currentToken = "";

    for (char ch : code) {
        // Skip white spaces
        if (isspace(ch)) {
            if (!currentToken.empty()) {
                tokens.push_back(currentToken);
                currentToken.clear();
            }
            continue;
        }

        // Handle operators
        if (isOperator(ch)) {
            if (!currentToken.empty()) {
                tokens.push_back(currentToken);
                currentToken.clear();
            }
            tokens.push_back(string(1, ch));  // Add operator as a token
            continue;
        }

        // Handle symbols
        if (isSymbol(ch)) {
            if (!currentToken.empty()) {
                tokens.push_back(currentToken);
                currentToken.clear();
            }
            tokens.push_back(string(1, ch));  // Add symbol as a token
            continue;
        }

        // Handle numbers
        if (isDigit(ch)) {
            currentToken += ch;  // Keep adding digits to currentToken
            continue;
        }

        // Handle keywords or identifiers
        if (isAlpha(ch)) {
            currentToken += ch;  // Keep adding letters to currentToken
            continue;
        }
    }

    // Add last token if any
    if (!currentToken.empty()) {
        tokens.push_back(currentToken);
    }

    return tokens;
}

int main() {
    string code = "int main() { int x = 5; return 0; }";
    
    vector<string> tokens = tokenize(code);

    // Print all tokens
    for (const string& token : tokens) {
        cout << token << '\n';
    }

    return 0;
}
