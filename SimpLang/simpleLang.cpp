#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "headers/lexer.hpp"
#include "headers/parser.hpp"
#include "headers/generator.hpp"

// Function to print an AST_NODE
void printASTNode(const AST_NODE *node, int depth = 0)
{
    if (node == nullptr)
    {
        return;
    }

    // Indentation for better readability
    std::string indent(depth * 4, ' ');

    // Print the type of the node
    cout << endl
         << endl;
    std::cout << indent << "Node Type: " << nodeToString(node->TYPE) << std::endl;

    // Print the value if it exists
    if (node->VALUE != nullptr)
    {
        std::cout << indent << "Value: " << *(node->VALUE) << std::endl;
    }

    // Print children nodes
    if (node->CHILD != nullptr)
    {
        std::cout << indent << "Child Node:" << std::endl;
        printASTNode(node->CHILD, depth + 1);
    }

    // Print sub-statements
    if (!node->SUB_STATEMENTS.empty())
    {
        std::cout << indent << "Sub-Statements:" << std::endl;
        for (const auto &sub : node->SUB_STATEMENTS)
        {
            printASTNode(sub, depth + 1);
        }
    }

    // Print reference status
    std::cout << indent << "Reference: " << (node->REFERENCE ? "true" : "false") << std::endl;
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        cout << "Please supply the source file";
        exit(1);
    }

    std::cout << "[*] COMPILING : " << argv[1] << std::endl;
    std::ifstream sourceFileStream(argv[1]);

    std::stringstream buffer;

    char temp;

    while (sourceFileStream.get(temp))
    {
        buffer << temp;
    }

    std::string sourceCode = buffer.str();
    sourceCode.append("\0");

    Lexer lexer(sourceCode);
    std::vector<Token *> tokens = lexer.tokenize();
    int counter = 0;
    if (tokens.back()->TYPE != TOKEN_EOF)
    {
        Token *EOFNode = new Token();
        EOFNode->TYPE = TOKEN_EOF;
        tokens.push_back(EOFNode);
    }

    for (Token *temp : tokens)
    {
        counter++;
        std::cout << counter << ") " << temp->VALUE << " " << typeToString(temp->TYPE) << std::endl;
    }
    // exit(0);

    Parser parser(tokens);
    AST_NODE *ROOT = parser.parse();

    printASTNode(ROOT);
    Generator generator(ROOT, argv[1]);
    generator.generate();

    return 0;
}
