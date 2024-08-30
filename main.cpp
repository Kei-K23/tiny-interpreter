#include <iostream>
#include <sstream>
#include <vector>
#include <memory>
#include <fstream> // For file handling
#include <string>

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

// Token structure to hold a token's type and its value
struct Token
{
    TokenType type;
    std::string value;
};

// The Lexer class turns an input string into tokens
class Lexer
{
public:
    Lexer(const std::string &source) : source(source), pos(0), currentChar(source[0]) {}

    // Get the next token from the input
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

            if (currentChar == '(')
            {
                advance();
                return {TokenType::L_PAREN, "("};
            }

            if (currentChar == ')')
            {
                advance();
                return {TokenType::R_PAREN, ")"};
            }

            return {TokenType::INVALID, std::string(1, currentChar)};
        }
        return {TokenType::END_OF_LINE, ""};
    }

private:
    std::string source; // Input string
    size_t pos;         // Current position in the string
    char currentChar;   // Current character

    // Move to the next character in the input
    void advance()
    {
        pos++;
        if (pos < source.length())
        {
            currentChar = source[pos];
        }
        else
        {
            currentChar = '\0'; // End of input
        }
    }

    // Handle number tokens
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

    // Handle identifier tokens (like variable names)
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

// The Parser class builds an AST from the tokens
class Parser
{
public:
    Parser(Lexer &lexer) : lexer(lexer), currentToken(lexer.getNextToken()) {}

    // Parse the expression and build the AST
    std::unique_ptr<ASTNode> parse()
    {
        return expr();
    }

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

            auto newNode = std::make_unique<ASTNode>(token);
            newNode->left = std::move(node);
            newNode->right = term();
            node = std::move(newNode);
        }
        return node;
    }

    // term handles multiplication and division
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

            auto newNode = std::make_unique<ASTNode>(token);
            newNode->left = std::move(node);
            newNode->right = factor();
            node = std::move(newNode);
        }
        return node;
    }

    // factor handles numbers and parentheses
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
            auto node = expr();
            eat(TokenType::R_PAREN);
            return node;
        }
        throw std::runtime_error("Invalid syntax: Expected number or '('");
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

// The Interpreter class walks the AST and computes the result
class Interpreter
{
public:
    Interpreter(Parser &parser) : parser(parser) {}

    // Interpret the expression and return the result
    int interpret()
    {
        std::unique_ptr<ASTNode> tree = parser.parse();
        return visit(tree.get());
    }

private:
    Parser &parser;

    // visit walks the AST and evaluates the expression
    int visit(ASTNode *node)
    {
        if (node->token.type == TokenType::NUMBER)
        {
            return std::stoi(node->token.value);
        }

        if (node->token.type == TokenType::PLUS)
        {
            return visit(node->left.get()) + visit(node->right.get());
        }
        if (node->token.type == TokenType::MINUS)
        {
            return visit(node->left.get()) - visit(node->right.get());
        }
        if (node->token.type == TokenType::MULTIPLY)
        {
            return visit(node->left.get()) * visit(node->right.get());
        }
        if (node->token.type == TokenType::DIVIDE)
        {
            return visit(node->left.get()) / visit(node->right.get());
        }
        throw std::runtime_error("Invalid syntax");
    }
};

// Main function: The entry point of the program
int main()
{
    std::string filename = "main.x";
    std::ifstream file(filename);

    if (!file)
    {
        std::cerr << "Error: Could not open the file" << filename << std::endl;
        return 1;
    }

    std::string source;

    while (std::getline(file, source))
        ;

    file.close();

    // Input string for the expression

    // Create the lexer with the input string
    Lexer lexer(source);

    // Create the parser with the lexer
    Parser parser(lexer);

    // Create the interpreter with the parser
    Interpreter interpreter(parser);

    try
    {
        // Interpret the expression and print the result
        int result = interpreter.interpret();
        std::cout << "Result: " << result << std::endl;
    }
    catch (std::exception &e)
    {
        // Handle any errors
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
