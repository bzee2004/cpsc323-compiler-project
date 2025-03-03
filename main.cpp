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
#include <filesystem>

#include "./app/lexer.cpp"

using namespace std;

int main() 
{
    vector<string> testArr = {
        "test1.rat25s",
        "test2.rat25s",
        "test3.rat25s",
    };

    for (string oFile : testArr) {
        ifstream sourceFile("test case/" + oFile);
        ofstream outputFile("test case/" + oFile + ".txt");

        if (!sourceFile) 
        {
            cerr << "Error: Cannot open input file." << endl;
            return 1;
        }

        outputFile << "Token\tLexeme\n-------------------\n";

        Token token;
        while ((token = lexer(sourceFile)).type != "EOF") 
        {
            cout << token.type << "\t" << token.lexeme << endl;
            outputFile << setw(15) << left << token.type << token.lexeme << endl;
        }

        sourceFile.close();
        outputFile.close();
    }
    
    return 0;
}

// ===================================================
// test1.rat25s OUTPUT:

// Token	Lexeme
// -------------------
// Identifier      upper
// Separator       )
// Identifier      a
// Operator        =
// Real    23.00
// Separator       ;
// Keyword endwhile
// ===================================================


// ===================================================
// test2.rat25s OUTPUT:

// Token	Lexeme
// -------------------
// Identifier     function
// Identifier     convertx
// Separator      (
// Identifier     fahr
// Identifier     integer
// Separator      )
// Separator      {
// Identifier     return
// Integer        5
// Operator       *
// Separator      (
// Identifier     fahr
// Operator       -
// Integer        32
// Separator      )
// Operator       /
// Integer        9
// Separator      ;
// Separator      }
// Identifier     integer
// Identifier     low
// Identifier     high
// Identifier     step
// Separator      ;
// Identifier     scan
// Separator      (
// Identifier     low
// Identifier     high
// Identifier     step
// Separator      )
// Separator      ;
// Keyword        while
// Separator      (
// Identifier     low
// Operator       <=
// Identifier     high
// Separator      )
// Separator      {
// Identifier     print
// Separator      (
// Identifier     low
// Separator      )
// Separator      ;
// Identifier     print
// Separator      (
// Identifier     convertx
// Separator      (
// Identifier     low
// Separator      )
// Separator      )
// Separator      ;
// Identifier     low
// Operator       =
// Identifier     low
// Operator       +
// Identifier     step
// Separator      ;
// Separator      }
// Identifier     endwhile
// ===================================================

// ===================================================
// test3.rat25s OUTPUT:

// Token	Lexeme
// -------------------
// Keyword        array
// Identifier     twoSum
// Operator       =
// Keyword        function
// Separator      (
// Identifier     numbers
// Keyword        array
// Identifier     target
// Keyword        integer
// Separator      )
// Separator      {
// Keyword        integer
// Identifier     index1
// Operator       =
// Integer        0
// Separator      ;
// Keyword        integer
// Identifier     index2
// Operator       =
// Identifier     numbers
// Identifier     length
// Operator       -
// Integer        1
// Separator      ;
// Keyword        while
// Separator      (
// Identifier     index1
// Operator       <
// Identifier     numbers
// Identifier     length
// Separator      )
// Separator      {
// Identifier     if
// Separator      (
// Identifier     target
// Operator       -
// Identifier     numbers
// Separator      [
// Identifier     index1
// Separator      ]
// Operator       <
// Identifier     numbers
// Separator      [
// Identifier     index2
// Separator      ]
// Separator      )
// Separator      {
// Operator       -
// Operator       -
// Identifier     index2
// Separator      ;
// Separator      }
// Identifier     else
// Identifier     if
// Separator      (
// Identifier     target
// Operator       -
// Identifier     numbers
// Separator      [
// Identifier     index1
// Separator      ]
// Operator       >
// Identifier     numbers
// Separator      [
// Identifier     index2
// Separator      ]
// Separator      )
// Separator      {
// Operator       +
// Operator       +
// Identifier     index1
// Separator      ;
// Separator      }
// Identifier     else
// Separator      {
// Identifier     return
// Separator      [
// Operator       +
// Operator       +
// Identifier     index1
// Operator       +
// Operator       +
// Identifier     index2
// Separator      ]
// Separator      ;
// Separator      }
// Separator      }
// Identifier     return
// Separator      [
// Separator      ]
// Separator      ;
// Separator      }
// Separator      ;
// Keyword        array
// Identifier     numbers
// Operator       =
// Separator      [
// Integer        2
// Integer        7
// Integer        11
// Integer        15
// Separator      ]
// Separator      ;
// Keyword        integer
// Identifier     target
// Operator       =
// Integer        9
// Keyword        array
// Identifier     result
// Operator       =
// Identifier     twoSum
// Separator      (
// Identifier     numbers
// Identifier     target
// Separator      )
// Separator      ;
// ===================================================