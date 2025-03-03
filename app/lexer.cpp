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

bool fsmIdentifier(const string &str)
{
    // FSM for Rat25s identifier: starts with a letter, followed by letters or digits
    int state = 0;
    for (char ch : str)
    {
        switch (state)
        {
        case 0: // Initial state
            if (isalpha(ch))
                state = 1; // First character must be a letter
            else
                return false;
            break;
        case 1: // Accept state - already saw a letter
            if (isalpha(ch) || isdigit(ch) || ch == '_')
                state = 1; // Can be followed by letters, digits, or underscores
            else
                return false;
            break;
        }
    }
    return state == 1 && !str.empty(); // Valid if we end in accept state and string is not empty
}

bool fsmInteger(const string &str)
{
    // FSM for Rat25s integer: one or more digits
    int state = 0;
    for (char ch : str)
    {
        switch (state)
        {
        case 0: // Initial state
            if (isdigit(ch))
                state = 1; // Transition to accept state on digit
            else
                return false; // Reject if not a digit
            break;
        case 1: // Accept state - already saw at least one digit
            if (isdigit(ch))
                state = 1; // Stay in accept state for more digits
            else
                return false; // Reject if not a digit
            break;
        }
    }
    return state == 1 && !str.empty(); // Valid if we end in accept state and string is not empty
}

bool fsmReal(const string &str)
{
    // FSM for Rat25s real number: digits followed by a decimal point and more digits
    int state = 0;
    for (char ch : str)
    {
        switch (state)
        {
        case 0: // Initial state
            if (isdigit(ch))
                state = 1; // First character must be a digit
            else
                return false;
            break;
        case 1: // Seen at least one digit before decimal point
            if (isdigit(ch))
                state = 1; // Continue accumulating digits
            else if (ch == '.')
                state = 2; // Found decimal point
            else
                return false;
            break;
        case 2: // Just seen decimal point
            if (isdigit(ch))
                state = 3; // Must have at least one digit after decimal
            else
                return false;
            break;
        case 3: // Accept state - seen digits after decimal point
            if (isdigit(ch))
                state = 3; // Continue accumulating fractional digits
            else
                return false;
            break;
        }
    }
    return state == 3; // Valid if we end in accept state (digits after decimal point)
}

Token lexer(ifstream &file)
{
    char ch;       // Current character being processed
    string lexeme; // Current lexeme being built
    Token token;   // Token to be returned

    bool commenting = false; // Flag to track if we're inside a comment block

    while (file.get(ch)) // Read characters until end of file
    {
        // Check for start of comment block [*
        if (ch == '[' && file.peek() == '*')
        {
            commenting = true;
            file.get(ch); // Consume the '*' character
        }
        // Check for end of comment block *]
        else if (ch == '*' && file.peek() == ']')
        {
            file.get(ch); // Consume the ']' character
            commenting = false;
            continue; // Skip to next iteration
        }

        // Skip whitespace and commented sections
        if (isspace(ch) || commenting)
            continue;

        lexeme = ch; // Start building new lexeme with current character

        // Check if character is a separator (e.g., '(', ')', ';', etc.)
        if (separators.count(ch))
        {
            return {"Separator", lexeme};
        }

        // Check for operators (e.g., '+', '-', '==', '<=', etc.)
        if (operators.count(lexeme) || (ch == '<' || ch == '>' || ch == '='))
        {
            file.get(ch); // Look ahead for possible double operators
            string doubleOp = lexeme + ch;
            if (operators.count(doubleOp))
            {
                return {"Operator", doubleOp}; // Return double operator (e.g., '<=')
            }
            else
            {
                file.unget(); // Put back character if not part of double operator
            }
            return {"Operator", lexeme}; // Return single operator
        }

        // Process identifiers and keywords (starting with a letter)
        if (isalpha(ch))
        {
            // Continue reading alphanumeric characters
            while (file.get(ch) && isalnum(ch))
            {
                lexeme += ch;
            }
            file.unget(); // Put back the last character that's not part of the lexeme

            // Check if lexeme is a keyword or identifier
            if (keywords.count(lexeme))
                return {"Keyword", lexeme};
            else if (fsmIdentifier(lexeme))
                return {"Identifier", lexeme};
        }
        // Process numbers (integers and reals)
        else if (isdigit(ch))
        {
            // Continue reading digits and possibly a decimal point
            while (file.get(ch) && (isdigit(ch) || ch == '.'))
            {
                lexeme += ch;
            }
            file.unget(); // Put back the last character that's not part of the number

            // Check if lexeme is a real number or integer
            if (fsmReal(lexeme))
                return {"Real", lexeme};
            if (fsmInteger(lexeme))
                return {"Integer", lexeme};
        }
    }
    // Return EOF token when file is completely read
    return {"EOF", ""};
}