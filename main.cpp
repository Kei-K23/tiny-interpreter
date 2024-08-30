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

// ASTNode represents a node in the Abstract Syntax Tree
struct ASTNode
{
    Token token;
    std::unique_ptr<ASTNode> left;
    std::unique_ptr<ASTNode> right;

    ASTNode(Token token) : token(token) {}
};

class Parser
{
private:
    Lexer &lexer;
    Token currentToken;

    // expr handles addition and subtraction
    std::unique_ptr<ASTNode> expr()
    {
        std::unique_ptr<ASTNode> node = term();

        while (currentToken.type == TokenType::PLUS || currentToken.type == TokenType::MINUS)
        {
            Token token = currentToken;
            if (token.type == TokenType::PLUS)
            {
                eat(TokenType::PLUS);
            }
            else if (token.type == TokenType::MINUS)
            {
                eat(TokenType::MINUS);
            }

            std::unique_ptr<ASTNode> newNode = std::make_unique<ASTNode>(token);
            newNode->left = std::move(node);
            newNode->right = term();
            node = std::move(newNode);
        }
        return node;
    }

    std::unique_ptr<ASTNode> term()
    {
        std::unique_ptr<ASTNode> node = factor();

        while (currentToken.type == TokenType::MULTIPLY || currentToken.type == TokenType::DIVIDE)
        {
            Token token = currentToken;
            if (token.type == TokenType::MULTIPLY)
            {
                eat(TokenType::MULTIPLY);
            }
            else if (token.type == TokenType::DIVIDE)
            {
                eat(TokenType::DIVIDE);
            }

            std::unique_ptr<ASTNode> newNode = std::make_unique<ASTNode>(token);
            newNode->left = std::move(node);
            newNode->right = factor();
            node = std::move(newNode);
        };
        return node;
    }

    // factor handle number and parentheses
    std::unique_ptr<ASTNode> factor()
    {
        Token token = currentToken;
        if (token.type == TokenType::NUMBER)
        {
            eat(TokenType::NUMBER);
            return std::make_unique<ASTNode>(token);
        }
        else if (token.type == TokenType::L_PAREN)
        {
            eat(TokenType::L_PAREN);
            std::unique_ptr<ASTNode> node = expr();
            eat(TokenType::R_PAREN);
            return node;
        }
        throw std::runtime_error("Invalid syntax");
    }

    // eat checks if the current token is what we expect and then gets the next token
    void eat(TokenType type)
    {
        if (currentToken.type == type)
        {
            currentToken = lexer.getNextToken();
        }
        else
        {
            throw std::runtime_error("Invalid syntax");
        }
    }
};