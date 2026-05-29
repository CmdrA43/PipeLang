#include <iostream>
#include <fstream>
#include <sstream>
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
            std::cout << "Reached end of file\n";
            return Token{TokenType::END_OF_FILE, ""};
        }
        std::string literal = "";
        // checking against literals
        
        // switch statement for punctuation
        switch(character){
            case '(':
                std::cout << "Got left parenthesis token\n";
                pos++;
                return Token{TokenType::LPAREN, "("};
                break;
            case ')':
                std::cout << "Got right parenthesis token\n";
                pos++;
                return Token{TokenType::RPAREN, "("};
                break;
            case '{':
                std::cout << "Got left brace token\n";
                pos++;
                return Token{TokenType::LBRACE, "{"};
                break;
            case '}':
                std::cout << "Got right brace token\n";
                pos++;
                return Token{TokenType::RBRACE, "}"};
                break;
            case '[':
                std::cout << "Got left bracket token\n";
                pos++;
                return Token{TokenType::LBRACKET, "["};
                break;
            case ']':
                std::cout << "Got right bracket token\n";
                pos++;
                return Token{TokenType::RBRACKET, "]"};
                break;
            case '"':
                std::cout << "Got quotation token\n";
                pos++;
                return Token{TokenType::QUOTATION, "\""};
                break;
                
            case ',':
                std::cout << "Got comma token\n";
                pos++;
                return Token{TokenType::COMMA, ","};
                break;
            case ';':
                std::cout << "Got semicolon token\n";
                pos++;
                return Token{TokenType::SEMICOLON, ";"};
                break;
            case ':':
                std::cout << "Got colon token\n";
                pos++;
                return Token{TokenType::COLON, ":"};
                break;
            case '.':
                std::cout << "Got dot token\n";
                pos++;
                return Token{TokenType::DOT, "."};
                break;
            
            case '>':
                if(text[pos+1] == '>'){
                    std::cout << "Got right arrow token\n";
                    pos += 2;
                    return Token{TokenType::RIGHT_ARROW, ">>"};
                }
                break;
            case '|':
                std::cout << "Got pipe token\n";
                pos++;
                return Token{TokenType::PIPE, "|"};
                break;
            
            case '=':
                std::cout << "Got equals token\n";
                pos++;
                return Token{TokenType::EQUALS, "="};
                break;
            case '+':
                std::cout << "Got plus token\n";
                pos++;
                return Token{TokenType::PLUS, "+"};
                break;
            case '-':
                std::cout << "Got minus token\n";
                pos++;
                return Token{TokenType::MINUS, "-"};
                break;
            case '*':
                std::cout << "Got star token\n";
                pos++;
                return Token{TokenType::STAR, "*"};
                break;
            case '/':
                std::cout << "Got slash token\n";
                pos++;
                return Token{TokenType::SLASH, "/"};
                break;
            case '&':
                std::cout << "Got ampersand token\n";
                pos++;
                return Token{TokenType::AMPERSAND, "&"};
                break;
            default:
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
    std::ifstream file("example.txt");
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();
    lexer.text = content;
    std::vector<Token> tokenList;
    Token t;
    while(t.type != TokenType::END_OF_FILE){
        t = lexer.getToken();
        tokenList.push_back(t);
    }

    return 0;
}