#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <filesystem>
#include "lexer.h"
#include "parser.h"

namespace fs = std::filesystem;

int main(int argc, char* argv[]){

    if(argc < 2){
        std::cerr << "Error: no input file specified.\n";
        std::cerr << "Usage: " << argv[0] << " <source_file_path>\n";
        return 1;
    };

    fs::path filePath(argv[1]);

    if(!fs::exists(filePath)){
        std::cerr << "Error: File '" << filePath.string() << "' does not exist.\n";
        return 1;
    }

    if (!fs::is_regular_file(filePath)) {
        std::cerr << "Error: '" << filePath.string() << "' is not a valid file.\n";
        return 1;
    }

    std::cout << "Successfully loaded target file: " << fs::absolute(filePath) << "\n";

    std::ifstream file(filePath.string());
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