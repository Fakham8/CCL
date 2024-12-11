#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <cctype>

using namespace std;

// Token Types
enum TokenType {
    T_INT, T_FLOAT, T_DOUBLE, T_STRING, T_BOOL, T_CHAR,
    T_ID, T_NUM, T_STR_LIT, T_AGAR, T_ELSE, T_RETURN,
    T_ASSIGN, T_PLUS, T_MINUS, T_MUL, T_DIV,
    T_LPAREN, T_RPAREN, T_LBRACE, T_RBRACE,
    T_SEMICOLON, T_GT, T_LOGICAL_AND, T_LOGICAL_OR,
    T_EQ, T_NEQ, T_EOF
};

// Token Structure
struct Token {
    TokenType type;
    string value;
};

// Lexer: Tokenizes the input source code
class Lexer {
private:
    string src;
    size_t pos;

public:
    Lexer(const string &src) : src(src), pos(0) {}

    vector<Token> tokenize() {
        vector<Token> tokens;
        while (pos < src.size()) {
            char current = src[pos];
            if (isspace(current)) { pos++; continue; }
            if (isdigit(current)) { tokens.push_back({T_NUM, consumeNumber()}); continue; }
            if (isalpha(current)) {
                string word = consumeWord();
                if (word == "int") tokens.push_back({T_INT, word});
                else if (word == "Agar") tokens.push_back({T_AGAR, word});
                else if (word == "else") tokens.push_back({T_ELSE, word});
                else if (word == "return") tokens.push_back({T_RETURN, word});
                else tokens.push_back({T_ID, word});
                continue;
            }
            if (current == '"') { tokens.push_back({T_STR_LIT, consumeString()}); continue; }
            switch (current) {
                case '=':
                    if (pos + 1 < src.size() && src[pos + 1] == '=') {
                        tokens.push_back({T_EQ, "=="});
                        pos++;
                    } else {
                        tokens.push_back({T_ASSIGN, "="});
                    }
                    break;
                case '(':
                    tokens.push_back({T_LPAREN, "("});
                    break;
                case ')':
                    tokens.push_back({T_RPAREN, ")"});
                    break;
                case '{':
                    tokens.push_back({T_LBRACE, "{"});
                    break;
                case '}':
                    tokens.push_back({T_RBRACE, "}"});
                    break;
                case ';':
                    tokens.push_back({T_SEMICOLON, ";"});
                    break;
                default:
                    cout << "Unexpected character: " << current << endl;
                    exit(1);
            }
            pos++;
        }
        tokens.push_back({T_EOF, ""});
        return tokens;
    }

private:
    string consumeNumber() {
        size_t start = pos;
        while (pos < src.size() && isdigit(src[pos])) pos++;
        return src.substr(start, pos - start);
    }

    string consumeWord() {
        size_t start = pos;
        while (pos < src.size() && isalnum(src[pos])) pos++;
        return src.substr(start, pos - start);
    }

    string consumeString() {
        pos++;  // Skip the opening quote
        size_t start = pos;
        while (pos < src.size() && src[pos] != '"') pos++;
        if (pos >= src.size()) { cerr << "Unterminated string literal" << endl; exit(1); }
        pos++;  // Skip the closing quote
        return src.substr(start, pos - start - 1);
    }
};

// Parser: Verifies the syntax and outputs parsed structures
class Parser {
private:
    vector<Token> tokens;
    size_t pos;

public:
    Parser(const vector<Token> &tokens) : tokens(tokens), pos(0) {}

    void parseProgram() {
        cout << "Parsed Program Structure:\n";
        while (tokens[pos].type != T_EOF) {
            parseStatement();
        }
        cout << "Parsing completed successfully!\n";
    }

private:
    void parseStatement() {
        if (tokens[pos].type == T_INT) {
            parseDeclaration();
        } else if (tokens[pos].type == T_ID) {
            parseAssignment();
        } else if (tokens[pos].type == T_AGAR) {
            parseIfStatement();
        } else if (tokens[pos].type == T_RETURN) {
            parseReturnStatement();
        } else {
            cout << "Syntax error: unexpected token " << tokens[pos].value << endl;
            exit(1);
        }
    }

    void parseDeclaration() {
        cout << "Declaration: ";
        expect(T_INT);
        cout << tokens[pos].value << endl; // Variable name
        expect(T_ID);
        expect(T_SEMICOLON);
    }

    void parseAssignment() {
        cout << "Assignment: " << tokens[pos].value << " = ";
        expect(T_ID);
        expect(T_ASSIGN);
        parseExpression();
        expect(T_SEMICOLON);
    }

    void parseIfStatement() {
        cout << "If Statement:\n";
        expect(T_AGAR);
        expect(T_LPAREN);
        parseExpression();
        expect(T_RPAREN);
        parseBlock();
        if (tokens[pos].type == T_ELSE) {
            cout << "Else Block:\n";
            pos++;
            parseBlock();
        }
    }

    void parseReturnStatement() {
        cout << "Return Statement: ";
        expect(T_RETURN);
        cout << tokens[pos].value << endl;
        expect(T_STR_LIT);
        expect(T_SEMICOLON);
    }

    void parseBlock() {
        expect(T_LBRACE);
        while (tokens[pos].type != T_RBRACE) {
            parseStatement();
        }
        expect(T_RBRACE);
    }

    void parseExpression() {
        if (tokens[pos].type == T_ID || tokens[pos].type == T_NUM) {
            cout << "Expression: " << tokens[pos].value << endl;
            pos++;
            if (tokens[pos].type == T_EQ || tokens[pos].type == T_NEQ) {
                cout << "Operator: " << tokens[pos].value << endl;
                pos++;
                cout << "Expression: " << tokens[pos].value << endl;
                pos++;
            }
        } else {
            cout << "Syntax error: unexpected token in expression: " << tokens[pos].value << endl;
            exit(1);
        }
    }

    void expect(TokenType expectedType) {
        if (tokens[pos].type != expectedType) {
            cout << "Syntax error: expected " << expectedType << ", found " << tokens[pos].value << endl;
            exit(1);
        }
        pos++;
    }
};

// Main function
int main() {
    string source = R"(
        int a;
        a = 5;
        Agar (a == 5) {
            return "Hello World";
        } else {
            return "Goodbye";
        }
    )";

    Lexer lexer(source);
    vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    parser.parseProgram();

    return 0;
}
