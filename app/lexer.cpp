// ===================================================
//Attached: Assignment 1
// ===================================================
//Program: Lexical Analyzer
// ===================================================
//Programmer: Ethan Nguyen, Brian Zee, Safe Gergis
//Class: CPSC 323 - Compilers and Program Languages
// ===================================================

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cctype>
#include <unordered_set>
#include <vector>

using namespace std;

struct Token 
{
    string type;
    string lexeme;
};

unordered_set<string> keywords = {"while", "endwhile", "for", "function", "scan", "integer", "array", "print"};
unordered_set<string> operators = {"<=", "=", "<", ">", "+", "-", "*", "/"};
unordered_set<char> separators = {'(', ')', ';', '[', ']', '{', '}'};


bool fsmIdentifier(const string& str) 
{
    int state = 0;
    for (char ch : str) 
    {
        switch (state) 
        {
            case 0:
                if (isalpha(ch)) state = 1;
                else return false;
                break;
            case 1:
                if (isalnum(ch)) state = 1;
                else return false;
                break;
        }
    }
    return state == 1;
}

bool fsmInteger(const string& str) 
{
    int state = 0;
    for (char ch : str) 
    {
        switch (state) 
        {
            case 0:
                if (isdigit(ch)) state = 1;
                else return false;
                break;
            case 1:
                if (isdigit(ch)) state = 1;
                else return false;
                break;
        }
    }
    return state == 1;
}

bool fsmReal(const string& str) 
{
    int state = 0;
    for (char ch : str) 
    {
        switch (state) 
        {
            case 0:
                if (isdigit(ch)) state = 1;
                else return false;
                break;
            case 1:
                if (isdigit(ch)) state = 1;
                else if (ch == '.') state = 2;
                else return false;
                break;
            case 2:
                if (isdigit(ch)) state = 3;
                else return false;
                break;
            case 3:
                if (isdigit(ch)) state = 3;
                else return false;
                break;
        }
    }
    return state == 3;
}

Token lexer(ifstream &file) 
{
    char ch;
    string lexeme;
    Token token;

    bool commenting = false;

    while (file.get(ch)) 
    {
        // Check for start of comment
        if (ch == '[' && file.peek() == '*') {
            commenting = true;
        }
        // Check for end of comment
        else if (ch == '*' && file.peek() == ']') {
            file.get(ch);
            commenting = false;
            continue;
        }

        if (isspace(ch) || commenting) continue;
        

        lexeme = ch;
        
        if (separators.count(ch)) 
        {
            return {"Separator", lexeme};
        }
        
        if (operators.count(lexeme) || (ch == '<' || ch == '>' || ch == '=')) 
        {
            file.get(ch);
            string doubleOp = lexeme + ch;
            if (operators.count(doubleOp)) 
            {
                return {"Operator", doubleOp};
            } 
            else 
            {
                file.unget();
            }
            return {"Operator", lexeme};
        }
        
        if (isalpha(ch)) 
        {
            while (file.get(ch) && isalnum(ch)) 
            {
                lexeme += ch;
            }
            file.unget();
            return {keywords.count(lexeme) ? "Keyword" : "Identifier", lexeme};
        }
        else if (isdigit(ch)) 
        {
            while (file.get(ch) && (isdigit(ch) || ch == '.')) 
            {
                lexeme += ch;
            }
            file.unget();
            if (fsmReal(lexeme)) return {"Real", lexeme};
            if (fsmInteger(lexeme)) return {"Integer", lexeme};
        }
    }
    return {"EOF", ""};
}