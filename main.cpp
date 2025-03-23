// ===================================================
// Attached: Assignment 1
// ===================================================
// Program: Lexical Analyzer
// ===================================================
// Programmer: Ethan Nguyen (13801), Brian Zee (13814), Safe Gergis (13814)
// Class: CPSC 323 - Compilers and Program Languages
// ===================================================

#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include "./app/lexical_analyzer/lexer.h"
#include "./app/syntax_analyzer/syntax.h"

using namespace std;

int main() 
{
    vector<string> testArr = {
        "test1.rat25s",
        "test2.rat25s",
        "test3.rat25s",
    };

    for (string oFile : testArr) {
        ifstream sourceFile("testcases/" + oFile);
        ofstream outputFileLexer("testcases/lexer_results/" + oFile + "-lexer.txt");

        if (!sourceFile) 
        {
            cerr << "Error: Cannot open input file." << endl;
            return 1;
        }

        outputFile << "Token\tLexeme\n-------------------\n";

        vector<Token> tokens;
        Token token;
        while ((token = lexer(sourceFile)).type != "EOF") 
        {
            tokens.push_back(token);
            // cout << token.type << "\t" << token.lexeme << endl;
            outputFileLexer << setw(15) << left << token.type << token.lexeme << endl;
        }
        tokens.push_back({"EOF", "", -1});

        sourceFile.close();
        outputFile.close();

        // Run Syntax Analyzer
        parse(tokens, oFile);
    }
    
    return 0;
}