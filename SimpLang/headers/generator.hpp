#ifndef __GENERATOR_H
#define __GENERATOR_H

#include "parser.hpp"
#include "lexer.hpp"
#include "../algorithms/infixToPostfix.hpp"
#include <fstream>
#include <vector>
#include <unordered_map>
#include <stack>
using namespace std;

class Generator
{

public:
    Generator(AST_NODE *ROOT, std::string filename)
    {
        AST_ROOT = ROOT;
        assemblyFile.open(filename + ".asm");
    }

    void writeSections()
    {
        assemblyFile << sectionText.str();
        assemblyFile << sectionBss.str();
        assemblyFile << sectionData.str();
        assemblyFile.close();
    }

    void createNewVariable(std::string VARIABLE_NAME, int *elemOffset)
    {
        variableVector.push_back(VARIABLE_NAME);
        sectionData << VARIABLE_NAME + " = ";
        sectionData << 0 << "\n";

        offsetCounter++;
        *elemOffset = offsetCounter * 4;

        cout << "*** creating new varible " << VARIABLE_NAME << endl;
    }

    void evaluteExpr(std::vector<AST_NODE *> &operations, int isCondition = 0, int label = 0)
    {

        vector<AST_NODE *> postfixOperations = infixToPostfix(operations);

        int isRegAUsed = 0;
        for (AST_NODE *CURRENT : postfixOperations)
        {

            switch (CURRENT->TYPE)
            {
            case NODE_INT:
            {
                sectionBss << indent;
                if (!isRegAUsed)
                {
                    loadRegister("A", *(CURRENT->VALUE), 1);
                    isRegAUsed = 1;
                }
                else
                {
                    loadRegister("B", *(CURRENT->VALUE), 1);
                }

                break;
            }
            case NODE_VARIABLE:
            {

                if (!isRegAUsed)
                {
                    sectionBss << indent + "lda  %" + *(CURRENT->VALUE) + "\n";
                    isRegAUsed = 1;
                }
                else
                {
                    loadRegister("B", *(CURRENT->VALUE), 0);
                }

                break;
            }
            case NODE_OP_ADD:
            {
                sectionBss << indent + "add\n";

                break;
            }
            case NODE_REL_EQUALS:
            {
                sectionBss << indent + "je \%label" + to_string(label) + "_1\n";
                sectionBss << indent + "jmp \%label" + to_string(label) + "_0\n";

                break;
            }
            case NODE_OP_SUB:
            {
                sectionBss << indent + "sub\n";

                break;
            }
            }
        }
    }

    void assignIdentifier(AST_NODE *VAR_ID)
    {

        if (VAR_ID->CHILD != NULL)
        {

            sectionBss << indent + "push A\n";
            sectionBss << indent + "ldi A " + *(VAR_ID->CHILD->VALUE) + "\n";
            sectionBss << indent + "sta %" + *(VAR_ID->VALUE) + "\n";
            sectionBss << indent + "pop A\n";
        }
        else
        {

            evaluteExpr(VAR_ID->SUB_STATEMENTS);
        }
        sectionBss << "\n";
    }

    void generateVARIABLE(AST_NODE *VAR_ID)
    {
        int offset;
        if (!VAR_ID->CHILD)
        {
            createNewVariable(*(VAR_ID->VALUE), &offset);
            return;
        }

        switch (VAR_ID->CHILD->TYPE)
        {
        case NODE_INT:
        {
            break;
        }
        case NODE_MATH:
        {
            evaluteExpr(VAR_ID->CHILD->SUB_STATEMENTS);
            break;
        }

        default:
        {
            std::cout << "[!] Generation Error : unidentidfied linkage to variable assignment : " << nodeToString(VAR_ID->CHILD->TYPE) << std::endl;
            exit(1);
        }
        }
    }
    void codeGenerator(AST_NODE *ROOT, int label = 0)
    {

        for (AST_NODE *CURRENT : ROOT->SUB_STATEMENTS)
        {

            switch (CURRENT->TYPE)
            {
            case NODE_VARIABLE:
            {
                generateVARIABLE(CURRENT);
                break;
            }
            case NODE_CONDITION:
            case NODE_ID:
            {
                if (CURRENT->TYPE == NODE_CONDITION)
                {
                    evaluteExpr(CURRENT->SUB_STATEMENTS, 1, label);
                    // sectionBss << indent + "sta %" + *(CURRENT->VALUE) << endl;
                }

                else if (CURRENT->CHILD->TYPE == NODE_MATH)
                {
                    evaluteExpr(CURRENT->CHILD->SUB_STATEMENTS);
                    sectionBss << indent + "sta %" + *(CURRENT->VALUE) << endl;
                }
                else
                {
                    assignIdentifier(CURRENT);
                }
                break;
            }
            case NODE_IF_BODY:
            {
                sectionBss << "\nlabel" + to_string(label) + "_1:\n";
                codeGenerator(CURRENT, label);
                sectionBss << "\nlabel" + to_string(label) + "_0:\n";
            
                break;
            }
            case NODE_IF:
            {
             
                codeGenerator(CURRENT, label + 1);
                break;
            }
            default:
            {
                std::cout << "[!] ERROR : FOUND : " << nodeToString(CURRENT->TYPE);
                std::cout << " IN GLOBAL SCOPE OF THE PROGRAM";
                exit(0);
            }
            }
        }
    }
    void generate()
    {
        stringReferenceCounter = 0;
        offsetCounter = 0;

        sectionText << ".text\n";

        sectionBss << "\n\nstart:\n\n";

        sectionData << "\n\n.data\n\n";

        codeGenerator(AST_ROOT);

        sectionBss << indent + "hlt";

        writeSections();
    }

private:
    void loadRegister(string reg, string val, int isIndirect)
    {
        if (isIndirect)
        {
            sectionBss << "ldi " + reg + " " + val + "\n";
        }
        else
        {
            sectionBss << indent + "push A\n";
            sectionBss << indent + "lda %" + val + "\n";
            sectionBss << indent + "mov " + reg + " A\n";
            sectionBss << indent + "pop A\n";
        }
    }
    stringstream sectionData;
    stringstream sectionText;
    stringstream sectionBss;
    int stringReferenceCounter;
    vector<string *> stringReferences;
    vector<string> variableVector;
    unordered_map<string, int> stringVariableVector;
    int initOffsetCounter;
    int offsetCounter;
    AST_NODE *AST_ROOT;
    ofstream assemblyFile;
    string indent = "    ";
};

#endif
