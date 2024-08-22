#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <algorithm>

using namespace std;
enum type
{
    TOKEN_DATA_TYPE,
    TOKEN_ID,
    TOKEN_VAL,
    TOKEN_EQUALS,
    TOKEN_SEMICOLON,
    TOKEN_LEFT_PAREN,
    TOKEN_RIGHT_PAREN,
    TOKEN_LEFT_CURLY_BRACE,
    TOKEN_RIGHT_CURLY_BRACE,
    TOKEN_KEYWORD,
    TOKEN_MATH,
    TOKEN_REL_EQUALS,
    TOKEN_EOF
};

struct Token
{
    enum type TYPE;
    string VALUE;
};

string typeToString(enum type TYPE)
{
    switch (TYPE)
    {
    case TOKEN_DATA_TYPE:
        return "TOKEN_DATA_TYPE";
    case TOKEN_ID:
        return "TOKEN_ID";
    case TOKEN_VAL:
        return "TOKEN_VAL";
    case TOKEN_EQUALS:
        return "TOKEN_EQUALS";
    case TOKEN_REL_EQUALS:
        return "TOKEN_REL_EQUALS";
    case TOKEN_SEMICOLON:
        return "TOKEN_SEMICOLON";
    case TOKEN_LEFT_PAREN:
        return "TOKEN_LEFT_PAREN";
    case TOKEN_RIGHT_PAREN:
        return "TOKEN_RIGHT_PAREN";
    case TOKEN_LEFT_CURLY_BRACE:
        return "TOKEN_LEFT_CURLY_BRACE";
    case TOKEN_RIGHT_CURLY_BRACE:
        return "TOKEN_RIGHT_CURLY_BRACE";
    case TOKEN_KEYWORD:
        return "TOKEN_KEYWORD";
    case TOKEN_EOF:
        return "TOKEN_EOF";
    case TOKEN_MATH:
        return "TOKEN_MATH";
    default:
        return "UNRECOGNIZED TOKEN";
    }
}

class Lexer
{
public:
    Lexer(string sourceCode)
    {
        source = sourceCode;
        cursor = 0;
        size = sourceCode.length();
        current = sourceCode.at(cursor);
    }

    char advance()
    {
        if (cursor < size)
        {
            char temp = current;
            cursor++;
            current = (cursor < size) ? source[cursor] : '\0';
            return temp;
        }
        else
        {
            return '\0';
        }
    }

    char peek(int offset = 0)
    {
        return (cursor + offset < size) ? source.at(cursor + offset) : '\0';
    }

    void skipWhitespace()
    {
        while (current == ' ' || current == '\t' || current == '\r' || current == '\n')
        {
            advance();
            line_number++;
        }
    }

    bool isAlpha(char ch)
    {
        return isalpha(ch) || ch == '_';
    }

    bool isAlnum(char ch)
    {
        return isalnum(ch) || ch == '_';
    }

    Token *tokenizeID_KEYWORD()
    {
        stringstream buffer;
        buffer << advance();

        while (isAlnum(current))
            buffer << advance();

        std::string value = buffer.str();
        Token *newToken = new Token();

        if (value == "int")
            newToken->TYPE = TOKEN_DATA_TYPE;
        else if (value == "if")
            newToken->TYPE = TOKEN_KEYWORD;
        else
            newToken->TYPE = TOKEN_ID;

        newToken->VALUE = value;
        return newToken;
    }

    Token *tokenizeINT()
    {
        std::stringstream buffer;

        while (isdigit(current))
        {
            buffer << advance();
        }
        Token *newToken = new Token();
        newToken->TYPE = TOKEN_VAL;
        newToken->VALUE = buffer.str();
        return newToken;
    }

    Token *tokenizeSpecial(enum type TYPE, string VAL)
    {
        Token *newToken = new Token();
        newToken->TYPE = TYPE;
        newToken->VALUE = VAL;
        return newToken;
    }

    std::vector<Token *> tokenize()
    {
        std::vector<Token *> tokens;

        while (cursor < size)
        {
            skipWhitespace();

            if (isAlpha(current))
            {
                tokens.push_back(tokenizeID_KEYWORD());
                continue;
            }

            if (isdigit(current))
            {
                tokens.push_back(tokenizeINT());
                continue;
            }

            switch (current)
            {
            case '=':
                cout << "{ PEEK }" << peek(1) << endl;
                if (peek(1) == '=')
                {
                    advance();
                    advance();
                    tokens.push_back(tokenizeSpecial(TOKEN_REL_EQUALS, "=="));
                }
                else
                {
                    tokens.push_back(tokenizeSpecial(TOKEN_EQUALS, string(1, advance())));
                }
                break;
            case '+':
            case '-':
                tokens.push_back(tokenizeSpecial(TOKEN_MATH, string(1, advance())));
                break;
            case '(':
                tokens.push_back(tokenizeSpecial(TOKEN_LEFT_PAREN, string(1, advance())));
                break;

            case ')':
                tokens.push_back(tokenizeSpecial(TOKEN_RIGHT_PAREN, string(1, advance())));
                break;
            case '{':
                tokens.push_back(tokenizeSpecial(TOKEN_LEFT_CURLY_BRACE, string(1, advance())));
                break;

            case '}':
                tokens.push_back(tokenizeSpecial(TOKEN_RIGHT_CURLY_BRACE, string(1, advance())));
                break;
            case ';':
                tokens.push_back(tokenizeSpecial(TOKEN_SEMICOLON, string(1, advance())));
                break;
            default:
                cout << "[!] LEXER ERROR: Unrecognized character '" << current << "' {" << line_number << " " << cursor << "}" << std::endl;

                exit(1);
            }
        }

        tokens.push_back(tokenizeSpecial(TOKEN_EOF, string(1, '\0')));
        return tokens;
    }

private:
    string source;
    int cursor;
    int size;
    char current;
    int line_number = 0;
};
