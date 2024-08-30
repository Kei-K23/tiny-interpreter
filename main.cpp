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

class Lexer
{
public:
    Lexer(std::string &source) : source(source), pos(0), currentChar(source[0]) {}

    Token getNextToken()
    {
        while (currentChar != '\0')
        {
            if (isspace(currentChar))
            {
                advance();
                continue;
            }

            if (isdigit(currentChar))
            {
                return number();
            }

            if (isalpha(currentChar))
            {
                return identifier();
            }

            if (currentChar == '+')
            {
                return {TokenType::PLUS, "+"};
            }

            if (currentChar == '-')
            {
                return {TokenType::MINUS, "-"};
            }

            if (currentChar == '*')
            {
                return {TokenType::MULTIPLY, "*"};
            }

            if (currentChar == '/')
            {
                return {TokenType::DIVIDE, "/"};
            }

            if (currentChar == '=')
            {
                return {TokenType::ASSIGN, "="};
            }

            if (currentChar == ';')
            {
                return {TokenType::SEMICOLON, ";"};
            }

            if (currentChar == '(')
            {
                return {TokenType::L_PAREN, "("};
            }

            if (currentChar == ')')
            {
                return {TokenType::R_PAREN, ")"};
            }

            return {TokenType::INVALID, std::string(1, currentChar)};
        }
        return {TokenType::END_OF_LINE, ""};
    }

private:
    std::string source;
    size_t pos;
    char currentChar;

    void advance()
    {
        pos++;
        if (pos > source.length())
        {
            currentChar = source[pos];
        }
        else
        {
            currentChar = '\0'; // Reach to the end of file
        }
    }

    Token number()
    {
        std::string result;
        while (isdigit(currentChar))
        {
            result += currentChar;
            advance();
        }
        return {TokenType::NUMBER, result};
    }

    Token identifier()
    {
        std::string result;
        while (isalnum(currentChar))
        {
            result += currentChar;
            advance();
        }
        return {TokenType::IDENTIFIER, result};
    }
};

// Main Program
int main()
{
    std::string source = "1 + 2 - 3;";
    Lexer lexer(source);
    return 0;
}
