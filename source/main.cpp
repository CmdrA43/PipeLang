#include <iostream>
#include <fstream>
#include <vector>

enum class TokenType{
    // reserved keywords
    ENTITY,
    COMPONENT,
    SYSTEM,
    IO,
    THREADED,
    FUSE,
    // grouping symbols
    LPAREN,
    RPAREN,
    LBRACE,
    RBRACE,
    LBRACKET,
    RBRACKET,
    QUOTATION,
    // delimiter symbols
    COMMA,
    SEMICOLON,
    COLON,
    DOT,
    // pipeline symbols
    RIGHT_ARROW,
    PIPE,
    // other
    EQUALS,
    PLUS,
    MINUS,
    STAR,
    SLASH,
    AMPERSAND,
    // special
    END_OF_FILE,
    IDENT,
    ERROR
};

struct Token{
    TokenType type;
    std::string value;
};

class Lexer{
    public:
    int pos = 0;
    std::string text;
    
    public:
    char forward(){
        if(text[pos] == ' ' || text[pos] == '\n'){
            pos++;
            return forward();
        }
        return text[pos];
    };
    
    Token getToken(){
        char character = forward();
        if(character == '\0'){
            return Token{TokenType::END_OF_FILE, ""};
        }
        std::string literal = "";
        // checking against literals
        if((character >= 'a' && character <= 'z') || (character >= 'A' && character <= 'Z')){
            while ((character >= 'a' && character <= 'z') || (character >= 'A' && character <= 'Z') || (character >= '0' && character <= '9')){
                literal += character;
                pos++;
                character = forward();
            }
            if(literal == "Entity"){
                std::cout << "Got Entity token\n";
                return Token{TokenType::ENTITY, literal};
            }
            else if(literal == "Component"){
                std::cout << "Got Component token\n";
                return Token{TokenType::COMPONENT, literal};
            }
            else if(literal == "System"){
                std::cout << "Got System token\n";
                return Token{TokenType::SYSTEM, literal};
            }
            // return as an identifier
            else{
                std::cout << "Got identifier token: \"" << literal << "\"\n";
                return Token{TokenType::IDENT, literal};
            }
        }
        // switch statement for punctuation
        switch(character){
            case '(':
                return Token{TokenType::LPAREN, "("};
                break;
            case ')':
                return Token{TokenType::RPAREN, "("};
                break;
            case '{':
                return Token{TokenType::LBRACE, "{"};
                break;
            case '}':
                return Token{TokenType::RBRACE, "}"};
                break;
            case '[':
                return Token{TokenType::LBRACKET, "["};
                break;
            case ']':
                return Token{TokenType::RBRACKET, "]"};
                break;
            case '"':
                return Token{TokenType::QUOTATION, "\""};
                break;
                
            case ',':
                return Token{TokenType::COMMA, ","};
                break;
            case ';':
                return Token{TokenType::SEMICOLON, ";"};
                break;
            case ':':
                return Token{TokenType::COLON, ":"};
                break;
            case '.':
                return Token{TokenType::DOT, "."};
                break;
            
            default:
            break;
        };
        
        // when all else fails, throw an error
        std::cout << "Unrecognized character: \"" << character << "\" at pos " << pos << std::endl;
        pos++;
        return Token{TokenType::ERROR, std::to_string(character)};
    };
};

Lexer lexer;

int main(){
    std::string example = "Entity[Position, Model] Player;";
    lexer.text = example;
    std::vector<Token> tokenList;
    Token t;
    while(t.type != TokenType::END_OF_FILE){
        t = lexer.getToken();
        tokenList.push_back(t);
    }

    return 0;
}