#include <iostream>
#include <sstream>
#include <vector>
#include <memory>
#include <string>

// Token and Lexer Definitions
enum class TokenType
{
    IDENTIFIER,
    NUMBER,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    ASSIGN,
    SEMICOLON,
    L_PAREN,
    R_PAREN,
    END_OF_LINE,
    INVALID
};

struct Token
{
    TokenType type;
    std::string value;
};

// Main Program
int main()
{
    return 0;
}
