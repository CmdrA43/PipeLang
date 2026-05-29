#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

// --- LEXER ---

enum class TokenType{
    // reserved keywords
    ENTITY,
    COMPONENT,
    SYSTEM,
    IO,
    THREADED,
    FUSE,
    // system priviliges
    READ_PRIV,
    WRITE_PRIV,
    EDIT_PRIV,
    // grouping symbols
    LPAREN,
    RPAREN,
    LBRACE,
    RBRACE,
    LBRACKET,
    RBRACKET,
    // delimiter symbols
    COMMA,
    SEMICOLON,
    COLON,
    DOT,
    // pipeline symbols
    RIGHT_ARROW,
    PIPE,
    // control flow
    IF,
    ELSE,
    // literals
    NUMBER,
    STRING,
    // other
    EQUALS,
    PLUS,
    MINUS,
    STAR,
    SLASH,
    AMPERSAND,
    LET,
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
    void skipWhitespace() {
        while (pos < text.length() && (text[pos] == ' ' || text[pos] == '\n'|| text[pos] == '\t' || text[pos] == '\r')) ++pos;
    }
    
    Token getToken(){
        skipWhitespace();
        if(text[pos] == '\0'){
            std::cout << "Reached end of file\n";
            return Token{TokenType::END_OF_FILE, ""};
        }
        std::string literal;
        
        // switch statement for punctuation
        switch(text[pos]){
            // groupers
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

            // delimiters
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

            // pipe specials
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

            // string literal handling
            case '"':
                std::cout << "Got string literal\n";
                pos++; // skip opening quote
                literal.clear();
                while (pos < text.length() && text[pos] != '"') {
                    if (text[pos] == '\\' && pos + 1 < text.length()) {
                        // simple escape handling
                        switch (text[pos + 1]) {
                            case '"':  literal += '"';  pos += 2; break;
                            case '\\': literal += '\\'; pos += 2; break;
                            case 'n':  literal += '\n'; pos += 2; break;
                            // add other escapes as needed
                            default:   literal += text[pos]; pos++; break;
                        }
                    } else {
                        literal += text[pos];
                        pos++;
                    }
                }
                if (pos < text.length()) {
                    pos++; // skip closing quote
                } else {
                    std::cout << "Warning: unterminated string literal\n";
                }
                return Token{TokenType::STRING, literal};
                break;
            
            // arithmetic/logic
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
            // checking against identifiers
                if(std::isalpha(text[pos]) || text[pos] == '_'){
                    while ((pos < text.length()) && (std::isalnum(text[pos]) || text[pos] == '_')){
                        literal += text[pos];
                        ++pos;
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
                    else if(literal == "read"){
                        std::cout << "Got read token\n";
                        return Token{TokenType::READ_PRIV, literal};
                    }
                    else if(literal == "write"){
                        std::cout << "Got write token\n";
                        return Token{TokenType::WRITE_PRIV, literal};
                    }
                    else if(literal == "edit"){
                        std::cout << "Got edit token\n";
                        return Token{TokenType::EDIT_PRIV, literal};
                    }
                    else if(literal == "if"){
                        std::cout << "Got if token\n";
                        return Token{TokenType::IF, literal};
                    }
                    else if(literal == "else"){
                        std::cout << "Got else token\n";
                        return Token{TokenType::ELSE, literal};
                    }
                    else if(literal == "let"){
                        std::cout << "Got let token\n";
                        return Token{TokenType::LET, literal};
                    }
                    // return as an identifier
                    else{
                        std::cout << "Got identifier token: \"" << literal << "\"\n";
                        return Token{TokenType::IDENT, literal};
                    }
                }
                // check if number
                if(std::isdigit(text[pos])){
                    std::string literal;
                    while(std::isdigit(text[pos]) || text[pos] == '.'){
                        literal += text[pos];
                        ++pos;
                    }
                    std::cout << "Got number literal\n";
                    return Token{TokenType::NUMBER, literal};
                }
            break;
        };
        
        // when all else fails, throw an error
        std::cout << "Unrecognized character: \"" << text[pos] << "\" at pos " << pos << std::endl;
        pos++;
        return Token{TokenType::ERROR, std::string(1, text[pos])};
    };
};

Lexer lexer;

// --- PARSER ---

int main(){
    std::ifstream file("../tests/example1.txt");
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();
    lexer.text = content;
    std::vector<Token> tokenList;
    Token t = lexer.getToken;
    while(t.type != TokenType::END_OF_FILE){
        tokenList.push_back(t);
        t = lexer.getToken();
    }

    return 0;
}