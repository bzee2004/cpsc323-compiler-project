#!/bin/bash
g++ main.cpp app/lexical_analyzer/lexer.cpp app/syntax_analyzer/syntax.cpp -o main.exe
./main.exe