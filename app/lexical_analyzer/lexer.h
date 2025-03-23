#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <fstream>
#include <unordered_set>
#include <vector>

struct Token {
    std::string type;
    std::string lexeme;
    int line;
};

Token lexer(std::ifstream &file);

#endif
