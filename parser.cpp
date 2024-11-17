#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <map>
#include <sstream>

using namespace std;

enum TokenType {
    T_INT, T_ID, T_NUM, T_IF, T_ELSE, T_RETURN,
    T_ASSIGN, T_PLUS, T_MINUS, T_MUL, T_DIV,
    T_LPAREN, T_RPAREN, T_LBRACE, T_RBRACE,
    T_SEMICOLON, T_GT, T_EOF,
    T_FLOAT , T_DOUBLE , T_STRING , T_CHAR,
    T_FLOAT_LITERAL , T_STRING_LITERAL , T_WHILE , T_FOR
   
};

struct Token {
    TokenType type;
    string value;
    int line;  // Line number where the token    k j k
};

class Lexer {
private:
    string src;
    size_t pos;
    int line;

public:
    Lexer(const string &src) {
        this->src = src;
        this->pos = 0;
        this->line = 0;
    }

    

    vector<Token> tokenize() {
        vector<Token> tokens;
        while (pos < src.size()) {
            char current = src[pos];

            if (current == '\n') {
                line++;
                pos++;
                continue;
            }

            
            if (current == '/' && src[pos + 1] == '/') {
                pos += 2;
                while (pos < src.size() && src[pos] != '\n') pos++;
                continue;

            }



            if (isspace(current)) {
                pos++;
                continue;
            }

            if (isdigit(current)) {
                
                tokens.push_back(Token{T_NUM, consumeNumber(), line});
                continue;
            }
           
           

            if (isalpha(current)) {
                string word = consumeWord();
                if (word == "int") tokens.push_back(Token{T_INT, word, line});
                else if (word == "if") tokens.push_back(Token{T_IF, word, line});
                else if (word == "else") tokens.push_back(Token{T_ELSE, word, line});
                else if (word == "float") tokens.push_back(Token{T_FLOAT, word, line});
                else if (word == "double") tokens.push_back(Token{T_DOUBLE, word, line});
                else if (word == "string") tokens.push_back(Token{T_STRING, word, line});
                else if (word == "char") tokens.push_back(Token{T_CHAR, word, line});
                else if (word == "return") tokens.push_back(Token{T_RETURN, word, line});
                // IMPLEMENT FOR WHILE 
                else if (word == "while") tokens.push_back(Token{T_WHILE, word, line});
                else if (word == "for") tokens.push_back(Token{T_FOR, word, line});
                
                else tokens.push_back(Token{T_ID, word, line});
                continue;
            }

            switch (current) {
                case '=': tokens.push_back(Token{T_ASSIGN, "=", line}); break;
                case '+': tokens.push_back(Token{T_PLUS, "+", line}); break;
                case '-': tokens.push_back(Token{T_MINUS, "-", line}); break;
                case '*': tokens.push_back(Token{T_MUL, "*", line}); break;
                case '/': tokens.push_back(Token{T_DIV, "/", line}); break;
                case '(': tokens.push_back(Token{T_LPAREN, "(", line}); break;
                case ')': tokens.push_back(Token{T_RPAREN, ")", line}); break;
                case '{': tokens.push_back(Token{T_LBRACE, "{", line}); break;
                case '}': tokens.push_back(Token{T_RBRACE, "}", line}); break;
                case ';': tokens.push_back(Token{T_SEMICOLON, ";", line}); break;
                case '>': tokens.push_back(Token{T_GT, ">", line}); break;
                default:
                    cout << "Unexpected character '" << current << "' at line " << line << endl;
                    exit(1);
            }
            pos++;
        }
        tokens.push_back(Token{T_EOF, "", line});
        return tokens;
    }

    string consumeNumber() {
        size_t start = pos;
        bool dotFlag = false;
        while (pos < src.size() && (isdigit(src[pos])) || (src[pos] == '.' && !dotFlag)) {
            if(src[pos] == '.'){
                dotFlag = true;
            }
            pos++;
        }
        return src.substr(start, pos - start);
    }


    string consumeWord() {
        size_t start = pos;
        while (pos < src.size() && isalnum(src[pos])) pos++;
        return src.substr(start, pos - start);
    }
};

class Parser {
public:
    Parser(const vector<Token> &tokens, const vector<string> &lines) {
        this->tokens = tokens;
        this->lines = lines;
        this->pos = 0;
        
    }

    void parseProgram() {
        while (tokens[pos].type != T_EOF) {
            parseStatement();
        }
        cout << "Parsing completed successfully! No Syntax Error" << endl;
    }

private:
    vector<Token> tokens;
    vector<string> lines;
    size_t pos;

    void parseStatement() {
        // print token and lines
       /* for (int i = 0; i < tokens.size(); i++) {
            cout << "Token: " << tokens[i].value << " at line " << tokens[i].line << endl;
        }
        */

        if (tokens[pos].type == T_INT) {
            parseDeclaration(T_INT);
        } else if (tokens[pos].type == T_ID) {
            parseAssignment();
        } else if (tokens[pos].type == T_IF) {
            parseIfStatement();
        } else if (tokens[pos].type == T_RETURN) {
            parseReturnStatement();
        } else if (tokens[pos].type == T_LBRACE) {
            parseBlock();
        }
        else if (tokens[pos].type == T_FLOAT) {
            parseDeclaration(T_FLOAT);
        }
        else if (tokens[pos].type == T_DOUBLE) {
            parseDeclaration(T_DOUBLE);
        }
        else if (tokens[pos].type == T_STRING) {
            parseDeclaration(T_STRING);
        }
        else if (tokens[pos].type == T_CHAR) {
            parseDeclaration(T_CHAR);

        }
        else if (tokens[pos].type == T_WHILE) {
            parseWhileStatement();
        }
        else if (tokens[pos].type == T_FOR) {
            parseForStatement();
        }
        
         else {
           int lineNum = tokens[pos].line;
              cout << "Syntax error at line " << lineNum <<endl;
        }
    }

    void parseBlock() {
        expect(T_LBRACE);
        while (tokens[pos].type != T_RBRACE && tokens[pos].type != T_EOF) {
            parseStatement();
        }
        expect(T_RBRACE);
    }

    void parseDeclaration(TokenType type) {
       
        expect(type);
        expect(T_ID);
        expect(T_SEMICOLON);
    }

   
    
    void parseAssignment() {
        expect(T_ID);
        expect(T_ASSIGN);
        parseExpression();
        expect(T_SEMICOLON);
    }

    void parseIfStatement() {
        expect(T_IF);
        expect(T_LPAREN);
        parseExpression();
        expect(T_RPAREN);
        parseStatement();
        if (tokens[pos].type == T_ELSE) {
            expect(T_ELSE);
            parseStatement();
        }
    }

    void parseReturnStatement() {
        expect(T_RETURN);
        parseExpression();
        expect(T_SEMICOLON);
    }

    void parseExpression() {
        parseTerm();
        while (tokens[pos].type == T_PLUS || tokens[pos].type == T_MINUS) {
            pos++;
            parseTerm();
        }
        if (tokens[pos].type == T_GT) {
            pos++;
            parseExpression();
        }
    }

    void parseTerm() {
        parseFactor();
        while (tokens[pos].type == T_MUL || tokens[pos].type == T_DIV) {
            pos++;
            parseFactor();
        }
    }

    void parseFactor() {
        if (tokens[pos].type == T_NUM || tokens[pos].type == T_ID || tokens[pos].type == T_FLOAT_LITERAL || tokens[pos].type == T_STRING_LITERAL) {
            pos++;
        } else if (tokens[pos].type == T_LPAREN) {
            expect(T_LPAREN);
            parseExpression();
            expect(T_RPAREN);
        } else {
            int lineNum = tokens[pos].line;
            cout << "Syntax error at line " << lineNum <<endl;
        }
    }

    void expect(TokenType type) {
        
        if (tokens[pos].type == type) {
          
            pos++;
        } else {
           int lineNum = tokens[pos].line;
           cout << "Syntax error at line " << lineNum <<endl;
        }
    }

    void parseWhileStatement() {
        expect(T_WHILE);
        expect(T_LPAREN);
        parseExpression();
        expect(T_RPAREN);
        parseStatement();
    }
    void parseForStatement() {

        expect(T_FOR);
        expect(T_LPAREN);
        parseAssignment();
       
        parseExpression();
        expect(T_SEMICOLON);
        expectIncDec();
        expect(T_RPAREN);
        parseBlock();
    }

     
    void expectIncDec() {
       
        if (tokens[pos].type == T_ID) {
            pos++;
        } else {
            int lineNum = tokens[pos].line;
            cout << "Syntax error at line " << lineNum <<endl;
        }
        if (tokens[pos].type == T_PLUS || tokens[pos].type == T_MINUS) {
            pos++;
        } else {
            int lineNum = tokens[pos].line;
            cout << "Syntax error at line " << lineNum <<endl;
        }
        if (tokens[pos].type == T_PLUS || tokens[pos].type == T_MINUS) {
            pos++;
        } else {
            int lineNum = tokens[pos].line;
            cout << "Syntax error at line " << lineNum <<endl;
        }
    }
    


   

   
};

int main() {
    string input = R"(
        int a;
        a = 5;
        int b;

        for(b = 4 ; b>5; b++)
        {
           b = b+ 1;
        }

        float c;
        c = 2.5;
        string ali;
        double p;
        // ass kjhva y9a832 %^%&%&t
        b =  a + 10;
       
        if (b > 10) {
            return b;
        } else {
            return 0;
        }
    )";

    // Split the input into lines
    vector<string> lines;
    istringstream stream(input);
    string line;
    while (getline(stream, line)) {
        lines.push_back(line);
    }

    

    Lexer lexer(input);
    vector<Token> tokens = lexer.tokenize();
    

    Parser parser(tokens, lines);
    parser.parseProgram();

    return 0;
}
