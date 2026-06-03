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
    PIPELINE,
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
                    else if(literal == "Threaded"){
                        std::cout << "Got Threaded token\n";
                        return Token{TokenType::THREADED, literal};
                    }
                    else if(literal == "Pipeline"){
                        std::cout << "Got Pipeline token\n";
                        return Token{TokenType::PIPELINE, literal};
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
                    else if(literal == "IO"){
                        std::cout << "Got IO token\n";
                        return Token{TokenType::IO, literal};
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

//declaration nodes
struct fieldNode{
    std::string name;
    std::string type;
};

struct componentDeclarationNode{
    std::string name;
    std::vector<fieldNode> fields;
};

struct entityDeclarationNode{
    std::string name;
    std::vector<std::string> components;
};

struct IODeclarationNode{
    std::string name;
    std::string type;
};

struct parameterNode{
    std::string privligeLevel;
    std::string name;
};

struct systemDeclarationNode{
    bool isThreaded;
    std::string name;
    std::vector<parameterNode> parameters;
    // make reference to expressions and statements eventually for actual execution block
};

struct pipelineDeclarationNode{
    std::vector<Token> pipe;
};

struct programNode{
    std::vector<componentDeclarationNode> components;
    std::vector<entityDeclarationNode> entities;
    std::vector<systemDeclarationNode> systems;
    std::vector<IODeclarationNode> IOs;
    
    pipelineDeclarationNode pipe;
};

class Parser{
    public:
    std::vector<Token> tokens;
    size_t pos = 0;
    
    Token peek(){
        if(pos < tokens.size()) return tokens[pos];
        return Token{TokenType::END_OF_FILE, ""};
    };
    
    Token advance(){
        if (pos < tokens.size()) return tokens[pos++];
        return Token{TokenType::END_OF_FILE, ""};
    };
    
    Token expect(TokenType type, const std::string& context = ""){
        Token t = peek();
        if(t.type != type){
            std::cerr << "Parse error at token " << pos
                      << ": expected " << tokenTypeName(type)
                      << " but got " << tokenTypeName(t.type)
                      << " (" << t.value << ")"
                      << (context.empty() ? "" : " in " + context)
                      << std::endl;
            exit(1);
        };
        return advance();
    };
    
    bool check(TokenType type){
        return peek().type == type;
    };
    
    bool match(TokenType type){
        if(check(type)){
            advance();
            return true;
        }
        return false;
    };
    
    // recursion parsing
    
    programNode parseProgram(){
        programNode program;
        while(!check(TokenType::PIPELINE) && !check(TokenType::END_OF_FILE)){
            parseDeclaration(program);
        }
        if(check(TokenType::PIPELINE)){
            parsePipeline(program);
        }
        expect(TokenType::END_OF_FILE, "end of file");
        return program;
    };
    
    void parseDeclaration(programNode& program){
        switch(peek().type){
            case TokenType::COMPONENT: program.components.push_back(parseComponent()); break;
            case TokenType::ENTITY: program.entities.push_back(parseEntity()); break;
            case TokenType::SYSTEM: program.systems.push_back(parseSystem()); break;
            case TokenType::IO: program.IOs.push_back(parseIO()); break;
            default:
                std::cerr << "Unexpected token " << tokenTypeName(peek().type) << " at start of declaration\n";
                exit(1);
        }
    };
    
    componentDeclarationNode parseComponent(){
        
        componentDeclarationNode component;
        
        expect(TokenType::COMPONENT, "component");
        Token name = expect(TokenType::IDENT, "component name");
        component.name = name.value;
        
        expect(TokenType::LBRACE, "component body");
        
        
        std::cout << "Component: " << name.value << "\n";
        // parse fields until closing brace
        while (!check(TokenType::RBRACE)) {
            fieldNode field;
            Token fieldName = expect(TokenType::IDENT, "field name");
            field.name = fieldName.value;
            
            expect(TokenType::COLON, "field type colon");
            Token fieldType = expect(TokenType::IDENT, "field type");
            field.type = fieldType.value;
            
            expect(TokenType::SEMICOLON, "after field");
            std::cout << "  field " << fieldName.value << " : " << fieldType.value << "\n";
            
            component.fields.push_back(field);
        }
        
        expect(TokenType::RBRACE, "end of component");
        expect(TokenType::SEMICOLON, "after component");
        return component;
    };
    
    entityDeclarationNode parseEntity(){
        entityDeclarationNode entity;
        
        expect(TokenType::ENTITY, "entity");
        Token name = expect(TokenType::IDENT, "component name");
        entity.name = name.value;
        
        expect(TokenType::EQUALS, "entity equals");
        expect(TokenType::LBRACE, "entity component list");
        
        std::cout << "Entity: " << name.value << " = { ";
        if (!check(TokenType::RBRACE)) {
            
            std::string componentName = expect(TokenType::IDENT, "component name").value;
            entity.components.push_back(componentName);
            
            std::cout << componentName;
            while (check(TokenType::COMMA)) {
                
                advance();
                
                componentName = expect(TokenType::IDENT, "component name").value;
                entity.components.push_back(componentName);
                
                std::cout << ", " << componentName;
            }
        }
        std::cout << " }" << std::endl;
        
        expect(TokenType::RBRACE, "end of entity");
        expect(TokenType::SEMICOLON, "after entity");
        return entity;
    };
    
    parameterNode parseParam(){
        parameterNode parameter;
        TokenType priv = peek().type;
        if (priv != TokenType::READ_PRIV && priv != TokenType::WRITE_PRIV && priv != TokenType::EDIT_PRIV) {
            std::cerr << "Expected access privilege (read/write/edit) but got " << tokenTypeName(priv) << "\n";
            exit(1);
        }
        advance();
        std::string access;
        if(priv == TokenType::READ_PRIV){
            access = "read";
            parameter.privligeLevel = "read";
        }
        if(priv == TokenType::WRITE_PRIV){
            access = "write";
            parameter.privligeLevel = "write";
        }
        if(priv == TokenType::EDIT_PRIV){
            access = "edit";
            parameter.privligeLevel = "edit";
        }
        // next is IDENT or IO.IDENT
        if (check(TokenType::IO)) {
            advance();
            expect(TokenType::DOT, "io dot");
            Token ioName = expect(TokenType::IDENT, "io name");
            parameter.name = ioName.value;
            std::cout << "  param: " << access << " io." << ioName.value << "\n";
        } else {
            Token compName = expect(TokenType::IDENT, "component name");
            parameter.name = compName.value;
            std::cout << "  param: " << access << " " << compName.value << "\n";
        }
        return parameter;
    };
    
    IODeclarationNode parseIO(){
        IODeclarationNode interface;
        expect(TokenType::IO, "IO identifier");
        expect(TokenType::DOT, "dot accessor");
        
        Token name = expect(TokenType::IDENT, "interface name");
        interface.name = name.value;
        
        expect(TokenType::COLON, "field type colon");
        Token type = expect(TokenType::IDENT, "type");
        interface.type = type.value;
        
        expect(TokenType::SEMICOLON, "after identifier");
        
        return interface;
    };
    
    systemDeclarationNode parseSystem() {
        systemDeclarationNode System;
        
        expect(TokenType::SYSTEM, "system keyword");
        if(match(TokenType::THREADED)){
            System.isThreaded = true;
            std::cout << "Creating threaded system\n";
        }
        Token name = expect(TokenType::IDENT, "system name");
        System.name = name.value;
        
        expect(TokenType::LPAREN, "systems parameters");
        std::cout << "System " << name.value << "(\n";
        
        if (!check(TokenType::RPAREN)) {
            System.parameters.push_back(parseParam());
            while (check(TokenType::COMMA)) {
                advance();
                System.parameters.push_back(parseParam());
            }
        }
        expect(TokenType::RPAREN, "end of params");
        
        expect(TokenType::LBRACE, "system body");
        while(!check(TokenType::RBRACE)){
            advance();
        }
        expect(TokenType::RBRACE, "body end");
        return System;
    };
    
    void parsePipeline(programNode& program){
        expect(TokenType::PIPELINE, "pipeline");
        while(peek().type != TokenType::SEMICOLON){
            program.pipe.pipe.push_back(tokens[pos]);
            advance();
        }
        expect(TokenType::SEMICOLON, "pipeline end");
    };
    
    private:
    std::string tokenTypeName(TokenType type){
        switch(type){
            case TokenType::ENTITY:
                return "entity";
                break;
            case TokenType::COMPONENT:
                return "component";
                break;
            case TokenType::SYSTEM:
                return "system";
                break;
            case TokenType::IO:
                return "IO";
                break;
            case TokenType::THREADED:
                return "threaded";
                break;
            case TokenType::FUSE:
                return "fuse";
                break;
            case TokenType::PIPELINE:
                return "pipeline";
                break;
                
            case TokenType::READ_PRIV:
                return "read";
                break;
            case TokenType::WRITE_PRIV:
                return "write";
                break;
            case TokenType::EDIT_PRIV:
                return "edit";
                break;
                
            case TokenType::LPAREN:
                return "(";
                break;
            case TokenType::RPAREN:
                return ")";
                break;
            case TokenType::LBRACE:
                return "{";
                break;
            case TokenType::RBRACE:
                return "}";
                break;
            case TokenType::LBRACKET:
                return "[";
                break;
            case TokenType::RBRACKET:
                return "]";
                break;
                
            case TokenType::COMMA:
                return ",";
                break;
            case TokenType::SEMICOLON:
                return ";";
                break;
            case TokenType::COLON:
                return ":";
                break;
            case TokenType::DOT:
                return ".";
                break;
                
            case TokenType::RIGHT_ARROW:
                return ">>";
                break;
            case TokenType::PIPE:
                return "|";
                break;
                
            case TokenType::IF:
                return "if";
                break;
            case TokenType::ELSE:
                return "else";
                break;
                
            case TokenType::NUMBER:
                return "number";
                break;
            case TokenType::STRING:
                return "string";
                break;
                
            case TokenType::EQUALS:
                return "=";
                break;
            case TokenType::PLUS:
                return "+";
                break;
            case TokenType::MINUS:
                return "-";
                break;
            case TokenType::STAR:
                return "*";
                break;
            case TokenType::SLASH:
                return "/";
                break;
            case TokenType::AMPERSAND:
                return "&";
                break;
            case TokenType::LET:
                return "let";
                break;
                
            case TokenType::END_OF_FILE:
                return "end of file";
                break;
            case TokenType::IDENT:
                return "identifier";
                break;
            case TokenType::ERROR:
                return "error";
                break;
        }
    };
};

Parser parser;

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