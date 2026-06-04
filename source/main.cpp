#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "lexer.h"
#include "parser.h"

int main(){
    std::ifstream file("example1.txt");
    std::stringstream buffer;
    
    buffer << file.rdbuf();
    std::string content = buffer.str();
    lexer.text = content;
    
    std::vector<Token> tokenList;
    Token t = lexer.getToken();
    
    while(t.type != TokenType::END_OF_FILE){
        tokenList.push_back(t);
        t = lexer.getToken();
    }
    
    parser.tokens = tokenList;
    parser.parseProgram();
    std::cout << "Finished parsing\n";
    
    
    return 0;
}