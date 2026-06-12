#include <iostream>
#include <vector>
#include <memory>

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

// pipeline nodes for parsing/enum for stage types

enum class StageType{
    SYSTEM,
    SUB_PIPELINE
};

struct pipelineSequenceNode;

struct stageNode{
    StageType type;
    std::string systemName;
    std::unique_ptr<pipelineSequenceNode> subPipe;
};

struct parallelGroupNode{
    std::vector<stageNode> stages;
};

struct pipelineSequenceNode{
    std::vector<parallelGroupNode> steps;
};

struct pipelineDeclarationNode{
    std::string name;
    pipelineSequenceNode seq;
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
            Token fieldType = expect(TokenType::TYPE, "field type");
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
        Token type = expect(TokenType::TYPE, "type");
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
        std::cout << ")\n";
        
        expect(TokenType::LBRACE, "system body");
        while(!check(TokenType::RBRACE)){
            advance();
        }
        expect(TokenType::RBRACE, "body end");
        return System;
    };
    
    pipelineSequenceNode parsePipelineSequence(){
        pipelineSequenceNode seq;
        std::cout << "  Seq: ";
        seq.steps.push_back(parseParallelGroup());
        while(match(TokenType::RIGHT_ARROW)){
            std::cout << " >> ";
            seq.steps.push_back(parseParallelGroup());
        }
        std::cout << "\n";
        return seq;
    };

    parallelGroupNode parseParallelGroup(){
        parallelGroupNode group;
        std::cout << "    Group: ";
        group.stages.push_back(parseStage());
        while(match(TokenType::PIPE)){
            std::cout << " | ";
            group.stages.push_back(parseStage());
        }
        return group;
    };

    stageNode parseStage(){
        stageNode stage;
        if(check(TokenType::LBRACE)){
            advance();
            std::cout << " { ";
            stage.type = StageType::SUB_PIPELINE;
            stage.subPipe = std::make_unique<pipelineSequenceNode>(parsePipelineSequence());
            expect(TokenType::RBRACE, "closing brace in pipeline");
            std::cout << " } ";
        } else {
            Token name = expect(TokenType::IDENT, "system identifier");
            stage.type = StageType::SYSTEM;
            stage.systemName = name.value;
            std::cout << name.value;
        }
        return stage;
    };

    void parsePipeline(programNode& program){
        expect(TokenType::PIPELINE, "pipeline");
        Token pipeName = expect(TokenType::IDENT, "name");
        std::cout << "Pipeline: " << pipeName.value << "\n";
        program.pipe.name = pipeName.value;
        expect(TokenType::COLON, "pipeline definition colon");
        program.pipe.seq = parsePipelineSequence();
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