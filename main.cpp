#include <iostream>
#include <string>
#include <memory>
#include <sstream>
#include <vector>

// Define different types of tokens our language can have
enum class TokenType
{
    IDENTIFIER,  // Variable names (not used in this basic example)
    NUMBER,      // Numbers like 1, 2, 3, etc.
    PLUS,        // The + operator
    MINUS,       // The - operator
    MULTIPLY,    // The * operator
    DIVIDE,      // The / operator
    ASSIGN,      // The = operator (not used in this basic example)
    SEMICOLON,   // The ; symbol (not used in this basic example)
    L_PAREN,     // The ( symbol
    R_PAREN,     // The ) symbol
    END_OF_LINE, // End of input
    INVALID      // Anything that doesn't match a valid token
};

// Token structure to hold a token's type and value
struct Token
{
    TokenType type;
    std::string value;
};

class Lexer
{
public:
    Lexer(std::string &source) : source(source), pos(0), currentChar(source[pos]) {}

    Token getNextToken()
    {
        while (currentChar != '\0')
        {
            // Skip the space
            if (isspace(currentChar))
            {
                // Skip to next character
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
                advance();
                return {TokenType::PLUS, "+"};
            }
            if (currentChar == '-')
            {
                advance();
                return {TokenType::MINUS, "-"};
            }
            if (currentChar == '*')
            {
                advance();
                return {TokenType::MULTIPLY, "*"};
            }
            if (currentChar == '/')
            {
                advance();
                return {TokenType::DIVIDE, "/"};
            }
            return {TokenType::INVALID, std::string(1, currentChar)};
        }
        return {TokenType::END_OF_LINE, ""};
    }

private:
    std::string source; // Source string or source
    size_t pos;         // Current position in the source string
    char currentChar;

    // Move to next character in the source string
    void advance()
    {
        // Increment position
        pos++;
        if (pos < source.length())
        {
            currentChar = source[pos];
        }
        else
        {
            // Indicate end of file
            currentChar = '\0';
        }
    }

    // Handle number token
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

    // Handle identifier tokens (like variable name)
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
