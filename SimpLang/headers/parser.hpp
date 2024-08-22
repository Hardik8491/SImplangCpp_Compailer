#ifndef __PARSER_H
#define __PARSER_H

#include "lexer.hpp"
#include <vector>
#include <stack>
#include <unordered_map>

using namespace std;

enum NODE_TYPE
{
    NODE_ROOT,
    NODE_ID,
    NODE_VARIABLE,
    NODE_RETURN,
    NODE_EXIT,
    NODE_GET,
    NODE_INT,
    NODE_STRING,
    NODE_IF,
    NODE_ELSE,
    NODE_CONDITION,
    NODE_REL_EQUALS,
    NODE_REL_NOTEQUALS,
    NODE_REL_LESSTHAN,
    NODE_REL_LESSTHANEQUALS,
    NODE_REL_GREATERTHAN,
    NODE_REL_GREATERTHANEQUALS,
    NODE_MATH,
    NODE_OP_ADD,
    NODE_OP_SUB,
    NODE_OP_MUL,
    NODE_OP_DIV,
    NODE_OP_MOD,
    NODE_CLEAR,
    NODE_SLEEP,
    NODE_IF_BODY
};

std::string nodeToString(enum NODE_TYPE TYPE)
{
    switch (TYPE)
    {
    case NODE_ROOT:
        return "NODE_ROOT";
    case NODE_ID:
        return "NODE_ID";
    case NODE_VARIABLE:
        return "NODE_VARIABLE";
    case NODE_RETURN:
        return "NODE_RETURN";
    case NODE_EXIT:
        return "NODE_EXIT";
    case NODE_GET:
        return "NODE_GET";
    case NODE_INT:
        return "NODE_INT";
    case NODE_STRING:
        return "NODE_STRING";
    case NODE_IF:
        return "NODE_IF";
    case NODE_IF_BODY:
        return "NODE_IF_BODY";
    case NODE_ELSE:
        return "NODE_ELSE";
    case NODE_CONDITION:
        return "NODE_CONDITION";
    case NODE_REL_EQUALS:
        return "NODE_REL_EQUALS";
    case NODE_REL_NOTEQUALS:
        return "NODE_REL_NOTEQUALS";
    case NODE_REL_LESSTHAN:
        return "NODE_REL_LESSTHAN";
    case NODE_REL_LESSTHANEQUALS:
        return "NODE_REL_LESSTHANEQUALS";
    case NODE_REL_GREATERTHAN:
        return "NODE_REL_GREATERTHAN";
    case NODE_REL_GREATERTHANEQUALS:
        return "NODE_REL_GREATERTHANEQUALS";
    case NODE_MATH:
        return "NODE_MATH";
    case NODE_OP_ADD:
        return "NODE_OP_ADD";
    case NODE_OP_SUB:
        return "NODE_OP_SUB";
    case NODE_OP_MUL:
        return "NODE_OP_MUL";
    case NODE_OP_DIV:
        return "NODE_OP_DIV";
    case NODE_OP_MOD:
        return "NODE_OP_MOD";
    case NODE_CLEAR:
        return "NODE_CLEAR";
    default:
        return "UNRECOGNIZED NODE";
    }
}

struct AST_NODE
{
    enum NODE_TYPE TYPE;
    std::string *VALUE;
    AST_NODE *CHILD;
    AST_NODE *LIMIT;
    std::vector<AST_NODE *> SUB_STATEMENTS;
    bool REFERENCE;
};

class Parser
{
public:
    Parser(std::vector<Token *> tokens)
    {
        parserTokens = tokens;
        index = 0;
        limit = parserTokens.size();
        current = parserTokens.at(index);
    }

    std::unordered_map<std::string, enum NODE_TYPE> operatorMap = {
        {"+", NODE_OP_ADD},
        {"-", NODE_OP_SUB},
        {"*", NODE_OP_MUL},
        {"/", NODE_OP_DIV},
        {"%", NODE_OP_MOD},
    };
    std::vector<std::string> errorTable = {
        "[!] Syntax Error : Expected an integer or a variable in sub script of the array ",
        "[!] Syntax Error : Expected an integer or a variable in the funtion arguments",
        "[!] Syntax Error : Expected the name of the parameter or reference token(@) ",
        "[!] Syntax Error : Expected either an integer or the name of a defined variable ",
        "[!] Syntax Error : Expected either an integer , defined variable or a mathematical statement"};

    Token *tokenSeek(int offset)
    {
        if (index + offset < limit)
            return parserTokens.at(index + offset);
        else
            return parserTokens.at(limit - 1);
    }

    Token *proceed(enum type TYPE)
    {
        if (current->TYPE != TYPE)
        {
            std::cout << "[!] SYNTAX ERROR ! Expected " << typeToString(TYPE) << " but got : " << typeToString(current->TYPE) << std::endl;
            exit(1);
        }
        index++;
        current = parserTokens.at(index);
        return current;
    }

    void subParseINT_ID(std::vector<AST_NODE *> &newNodeVector)
    {
        switch (current->TYPE)
        {
        case TOKEN_VAL:
        {
            newNodeVector.push_back(parseINT());
            break;
        }
        case TOKEN_ID:
        {
            newNodeVector.push_back(parseID_RHS());
            break;
        }
        default:
        {
            raiseSyntaxError(4);
        }
        }
    }

    void subParseINT_ID_MATH(std::vector<AST_NODE *> &newNodeVector)
    {
        switch (current->TYPE)
        {
        case TOKEN_VAL:
        {
            newNodeVector.push_back(parseINT());
            break;
        }
        case TOKEN_ID:
        {
            newNodeVector.push_back(parseID_RHS());
            break;
        }
        case TOKEN_MATH:
        {
            if (current->VALUE != "-")
            {
                std::cout << "[!] Unsupported mathematical operator while creating the array : " << current->VALUE;
                exit(1);
            }
            newNodeVector.push_back(parseINT(true));
            break;
        }
        default:
        {
            raiseSyntaxError(5);
        }
        }
    }
    void raiseSyntaxError(int errorCode = 0)
    {
        if (errorCode != 0)
            std::cout << errorTable[errorCode - 1];
        else
            std::cout << "[!] Syntax Error !" << std::endl;

        exit(1);
    }

    void raiseUnidentifiedTokenError()
    {
        std::cout << "[!] Syntax Error ! Unindentified Token : " << typeToString(current->TYPE) << std::endl;
        exit(1);
    }

    void raiseUnexpectedTokenError()
    {
        std::cout << "[!] Syntax Error ! Unexpected Token : " << typeToString(current->TYPE) << std::endl;
        exit(1);
    }

    void inScopeParsing(AST_NODE *newNode)
    {
        proceed(TOKEN_SEMICOLON);
        switch (current->TYPE)
        {
        case TOKEN_ID:
        {

            newNode->SUB_STATEMENTS.push_back(parseID());
            break;
        }
        case TOKEN_KEYWORD:
        {
            newNode->SUB_STATEMENTS.push_back(parseKEYWORD());
            break;
        }
        default:
        {
            raiseSyntaxError();
        }
        }
        if (current->TYPE != TOKEN_SEMICOLON)
            raiseUnexpectedTokenError();
    }

    AST_NODE *parseINT(bool negativeNumber = false)
    {
        if (negativeNumber)
            proceed(TOKEN_MATH);
        cout << current->TYPE << " " << current->VALUE << endl;
        if (current->TYPE != TOKEN_VAL)
        {
            cerr << "TKN val not found" << endl;
            raiseSyntaxError();
        }

        AST_NODE *newNode = new AST_NODE();
        newNode->TYPE = NODE_INT;
        if (negativeNumber)
        {
            int toNegate = std::stoi(current->VALUE);
            toNegate *= -1;
            current->VALUE = std::to_string(toNegate);
        }
        newNode->VALUE = &current->VALUE;
        proceed(TOKEN_VAL);
        return newNode;
    }

    AST_NODE *parseID_RHS(bool isReference = false)
    {
        std::string *buffer = &current->VALUE;
        proceed(TOKEN_ID);

        AST_NODE *newNode = new AST_NODE();

        newNode->VALUE = buffer;
        newNode->TYPE = NODE_VARIABLE;

        if (isReference)
            newNode->REFERENCE = isReference;

        return newNode;
    }

    AST_NODE *parseOPERATOR()
    {
        AST_NODE *newNode = new AST_NODE();
        newNode->TYPE = operatorMap[current->VALUE];
        proceed(TOKEN_MATH);
        return newNode;
    }

    AST_NODE *parseMATH()
    {
        AST_NODE *newNode = new AST_NODE();
        newNode->TYPE = NODE_MATH;

        subParseINT_ID(newNode->SUB_STATEMENTS);
        newNode->SUB_STATEMENTS.push_back(parseOPERATOR());
        subParseINT_ID(newNode->SUB_STATEMENTS);

        while (current->TYPE == TOKEN_MATH)
        {
            newNode->SUB_STATEMENTS.push_back(parseOPERATOR());
            subParseINT_ID(newNode->SUB_STATEMENTS);
        }
        return newNode;
    }

    AST_NODE *parseVariableDeclaration()
    {
        proceed(TOKEN_DATA_TYPE);
        string *buffer = &current->VALUE;
        AST_NODE *newNode = new AST_NODE();
        newNode->VALUE = buffer;
        newNode->TYPE = NODE_VARIABLE;
        proceed(TOKEN_ID);

        return newNode;
    }

    AST_NODE *parseID()
    {

        std::string *buffer = &current->VALUE;
        AST_NODE *newNode = new AST_NODE();
        newNode->VALUE = buffer;

        newNode->TYPE = NODE_ID;
        proceed(TOKEN_ID);

        proceed(TOKEN_EQUALS);

        switch (current->TYPE)
        {
        case TOKEN_VAL:
        {
            newNode->CHILD = (tokenSeek(1)->TYPE == TOKEN_MATH) ? parseMATH() : parseINT();

            break;
        }
        case TOKEN_ID:
        {
            if (tokenSeek(1)->TYPE == TOKEN_MATH)
                newNode->CHILD = parseMATH();
            else
                newNode->CHILD = parseID_RHS();
            break;
        }
        case TOKEN_MATH:
        {
            if (current->VALUE != "-")
            {
                std::cout << "[!} Syntax Error : Unindentified Token : " << typeToString(current->TYPE) << std::endl;
                exit(1);
            }
            newNode->CHILD = parseINT(true);
            break;
        }

        default:
        {
            std::cout << "[!] SYNTAX ERROR : Unidentified Token : " << typeToString(current->TYPE) << std::endl;
            exit(1);
        }
        }

        return newNode;
    }

    AST_NODE *parseCONDITION()
    {
        AST_NODE *newNODE = new AST_NODE();
        newNODE->TYPE = NODE_CONDITION;
        switch (current->TYPE)
        {
        case TOKEN_ID:
        {
            if (tokenSeek(1)->TYPE == TOKEN_MATH)
                newNODE->SUB_STATEMENTS.push_back(parseMATH());
            else
                newNODE->SUB_STATEMENTS.push_back(parseID_RHS());

            break;
        }
        case TOKEN_VAL:
        {
            newNODE->SUB_STATEMENTS.push_back((tokenSeek(1)->TYPE == TOKEN_MATH) ? parseMATH() : parseINT());
            break;
        }
        default:
        {
            std::cout << "[!] SYNTAX ERROR : Unexpected Token : " << typeToString(current->TYPE) << std::endl;
            exit(1);
        }
        }
        AST_NODE *operatorNODE = new AST_NODE();
        switch (current->TYPE)
        {
        case TOKEN_REL_EQUALS:
        {
            operatorNODE->TYPE = NODE_REL_EQUALS;
            proceed(TOKEN_REL_EQUALS);
            break;
        }
        default:
        {
            std::cout << "[!] SYNTAX ERROR : Unexpected Token : " << typeToString(current->TYPE) << " Expected a relational operator " << std::endl;
            exit(1);
        }
        }
        newNODE->SUB_STATEMENTS.push_back(operatorNODE);
        switch (current->TYPE)
        {
        case TOKEN_ID:
        {
            if (tokenSeek(1)->TYPE == TOKEN_MATH)
                newNODE->SUB_STATEMENTS.push_back(parseMATH());
            else
                newNODE->SUB_STATEMENTS.push_back(parseID_RHS());
            break;
        }
        case TOKEN_VAL:
        {
            newNODE->SUB_STATEMENTS.push_back((tokenSeek(1)->TYPE == TOKEN_MATH) ? parseMATH() : parseINT());
            break;
        }
        default:
        {
            std::cout << "[!] SYNTAX ERROR : Unexpected Token : " << typeToString(current->TYPE) << std::endl;
            exit(1);
        }
        }

        return newNODE;
    }

    AST_NODE *parseIfBody()
    {
        AST_NODE *newNODE = new AST_NODE();

        newNODE->TYPE = NODE_IF_BODY;
        scopeLog.push(0);

        while (current->TYPE != TOKEN_RIGHT_CURLY_BRACE)
        {
            switch (current->TYPE)
            {
            case TOKEN_DATA_TYPE:
            {

                newNODE->SUB_STATEMENTS.push_back(parseVariableDeclaration());
                break;
            }
            case TOKEN_ID:
            {
                newNODE->SUB_STATEMENTS.push_back(parseID());
                break;
            }
            case TOKEN_KEYWORD:
            {

                newNODE->SUB_STATEMENTS.push_back(parseKEYWORD());
                break;
            }

            default:
            {
                std::cout << "[!] SYNTAX ERROR " << typeToString(current->TYPE) << " " << current->VALUE << std::endl;
                exit(1);
            }
            }

            proceed(TOKEN_SEMICOLON);
        }
        return newNODE;
    }

    AST_NODE *parseIF()
    {
        proceed(TOKEN_KEYWORD);

        AST_NODE *newNODE = new AST_NODE();
        newNODE->TYPE = NODE_IF;

        proceed(TOKEN_LEFT_PAREN);

        newNODE->SUB_STATEMENTS.push_back(parseCONDITION());

        proceed(TOKEN_RIGHT_PAREN);
        proceed(TOKEN_LEFT_CURLY_BRACE);
        newNODE->SUB_STATEMENTS.push_back(parseIfBody());

        proceed(TOKEN_RIGHT_CURLY_BRACE);

        return newNODE;
    }

    AST_NODE *parseKEYWORD()
    {

        if (current->VALUE == "if")
            return parseIF();
        else
        {
            std::cout << "[!] SYNTAX ERROR , UNDEFINED KEYWORD " << std::endl;
            exit(1);
        }
    }

    AST_NODE *parse()
    {
        AST_NODE *ROOT = new AST_NODE();
        ROOT->TYPE = NODE_ROOT;
        scopeLog.push(0);

        while (current->TYPE != TOKEN_EOF)
        {

            switch (current->TYPE)
            {
            case TOKEN_DATA_TYPE:
            {

                ROOT->SUB_STATEMENTS.push_back(parseVariableDeclaration());
                proceed(TOKEN_SEMICOLON);
                break;
            }
            case TOKEN_ID:
            {
                ROOT->SUB_STATEMENTS.push_back(parseID());
                proceed(TOKEN_SEMICOLON);
                break;
            }
            case TOKEN_KEYWORD:
            {

                ROOT->SUB_STATEMENTS.push_back(parseKEYWORD());
                break;
            }

            default:
            {
                std::cout << "[!] SYNTAX ERROR " << std::endl;
                exit(1);
            }
            }
        }

        return ROOT;
    }

private:
    int limit;
    int index;
    int bufferScope;
    Token *current;
    vector<Token *> parserTokens;
    stack<int> scopeLog;
};

#endif
