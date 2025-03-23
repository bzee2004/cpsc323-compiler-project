#include "syntax.h"
using namespace std;

vector<Token> tokens;
int currentTokenIndex = 0;
bool debug = true;
ofstream outputFile;

Token peek() 
{
    if (currentTokenIndex < tokens.size()) 
    {
        return tokens[currentTokenIndex];
    }
    return 
    {"EOF", "", -1};
}

Token getNextToken() 
{
    if (currentTokenIndex < tokens.size()) 
    {
        return tokens[currentTokenIndex++];
    }
    return 
    {"EOF", "", -1};
}

void match(string expectedType) 
{
    Token token = peek();

    outputFile << "Token: " << token.type << " Lexeme: " << token.lexeme << endl;

    if (token.type == expectedType) 
    {
        getNextToken();
    } 
    else 
    {
        error("Expected " + expectedType + " but found " + token.lexeme);
    }
}

void error(string message) 
{
    Token token = peek();
    cerr << "Syntax Error: " << message << " at line " << token.line << endl;
    outputFile << "Syntax Error: " << message << " at line " << token.line << endl;
    exit(1);
}


void Rat25s()
{
    Token token = peek();
    if (debug) outputFile << "<Rat25S> -> $$ <Opt Function Definitions> $$ <Opt Declaration List> $$ <Statement List> $$\n";

    match("Separator");         // $$
    OptFunctionDefs();
    match("Separator");         // $$
    OptDeclarationList();
    match("Separator");         // $$
    StatementList();
    match("Separator");         // $$
}


void OptFunctionDefs() {
    Token token = peek();

    if (token.lexeme != "$$") {
        if (debug) outputFile << "<Opt Function Definitions> -> <Function Definitions>\n";
        FunctionDefs();
    }
    else {
        if (debug) outputFile << "<Opt Function Definitions> -> <Empty>\n";
        Empty();
    }
}


void FunctionDefs() {
    Token token = peek();
    if (debug) outputFile << "<Function Definitions> -> <Function> | <Function> <Function Definitions>\n";
    Function();
    FunctionDefsPrime();
}


void FunctionDefsPrime() {
    Token token = peek();
    if (token.lexeme != "$$") {
        if (debug) outputFile << "<Function Definitions'> -> <Function>\n";
        FunctionDefs();
    }
    else {
        if (debug) outputFile << "<Function Definitions'> -> <Empty>\n";
        Empty();
    }
}


void Function() {
    if (debug) outputFile << "<Function> -> function <Identifier> ( <Opt Parameter List> ) <Opt Declaration List> <Body>\n";

    match("Keyword");       // function
    match("Identifier");

    match("Separator");     // (
    OptParameterList();
    match("Separator");     // )

    OptDeclarationList();
    Body();
}   


void OptParameterList() {
    Token token = peek();
    if (token.lexeme != ")") {
        if (debug) outputFile << "<Opt Parameter List> -> <Parameter List>\n";
        ParameterList();
    }
    else {
        if (debug) outputFile << "<Opt Parameter List> -> <Empty>\n";
        Empty();
    }
}

void ParameterList() {
    if (debug) outputFile << "<Parameter List> -> <Parameter> <Parameter List'>\n";
    Parameter();
    ParameterListPrime();
}

void ParameterListPrime() {
    Token token = peek();
    if (token.lexeme == ",") {
        if (debug) outputFile << "<Parameter List'> -> , <Parameter List>\n";

        match("Separator");
        ParameterList();
    }
    else {
        if (debug) outputFile << "<Parameter List'> -> <Empty>\n";
        Empty();
    }
}


void Parameter() {
    if (debug) outputFile << "<Parameter> -> <IDs> <Qualifier>\n";
    IDS();
    Qualifier();
}


void Qualifier() {
    Token token = peek();
    if (token.lexeme == "integer") {
        if (debug) outputFile << "<Qualifier> -> integer\n";
        match("Keyword");
    }
    else if (token.lexeme == "boolean") {
        if (debug) outputFile << "<Qualifier> -> boolean\n";
        match("Keyword");
    }
    else if (token.lexeme == "real") {
        if (debug) outputFile << "<Qualifier> -> real\n";
        match("Keyword");
    }
    else if (token.lexeme == "array") {
        if (debug) outputFile << "<Qualifier> -> array\n";
        match("Keyword");
    }
    else {
        error("Expected integer, boolean, real, or array, but found " + token.lexeme + "\n");
    }
}


void Body() {
    Token token = peek();
    if (token.lexeme == "{") {
        match("Separator");
        StatementList();
        match("Separator");
    }
    else {
        error("Expected '{' but found " + token.lexeme + "\n");
    }
}


void OptDeclarationList() {
    Token token = peek();
    if (token.lexeme != "$$" && token.lexeme != "{") {
        if (debug) outputFile << "<Opt Declaration List> -> <Declaration List>\n";
        DeclarationList();
    }
    else {
        if (debug) outputFile << "<Opt Declaration List> -> <Empty>\n";
        Empty();
    }
}


void DeclarationList() {
    if (debug) outputFile << "<Declaration List> -> <Declaration> ; <Declaration List'>\n";
    Declaration();
    match("Separator");
    DeclarationListPrime();
}

void DeclarationListPrime() {
    Token token = peek();
    if (token.lexeme != "$$") {
        if (debug) outputFile << "<Declaration List'> -> <Declaration List>\n";
        DeclarationList();
    }
    else {
        if (debug) outputFile << "<Declaration List'> -> <Empty>\n";
        Empty();
    }
}


void Declaration() {
    if (debug) outputFile << "<Declaration> -> <Qualifier> <IDs>\n";
    Qualifier();
    IDS();
}


void IDS() {
    Token token = peek();
    if (token.type == "Identifier") {
        if (debug) outputFile << "<IDS> -> <Identifier> <IDS'>\n";
        match("Identifier");
        IDSPrime();
    }
    else {
        error("Expected Identifier, but found " + token.lexeme + "\n");
    }
}


void IDSPrime() {
    Token token = peek();
    if (token.lexeme == ",") {
        if (debug) outputFile << "<IDS'> -> , <IDS>\n";
        match("Separator");
        IDS();
    }
    else {
        if (debug) outputFile << "<IDS'> -> <Empty>\n";
        Empty();
    }
}


void StatementList() {
    if (debug) outputFile << "<Statement List> -> <Statement> <Statement List'>\n";
    Statement();
    StatementListPrime();
}

void StatementListPrime() {
    string lexeme = peek().lexeme;
    if (peek().type == "Identifier" || lexeme == "{" || lexeme == "if" || lexeme == "return" || lexeme == "print" || lexeme == "scan" || lexeme == "while") {
        if (debug) outputFile << "<Statement List'> -> <Statement List>\n";
        StatementList();
    }
    else {
        if (debug) outputFile << "<Statement List'> -> <Empty>\n";
        Empty();
    }
}


void Statement() 
{
    Token token = peek();

    if (token.type == "Identifier") 
    {
        if (debug) outputFile << "<Statement> -> <Assign>\n";
        Assign();
    }
    else if (token.lexeme == "{") {
        if (debug) outputFile << "<Statement> -> <Compound>\n";
        Compound();
    }
    else if (token.lexeme == "if") {
        if (debug) outputFile << "<Statement> -> <If>\n";
        If();
    }
    else if (token.lexeme == "return") {
        if (debug) outputFile << "<Statement> -> <Return>\n";
        Return();
    }
    else if (token.lexeme == "print") {
        if (debug) outputFile << "<Statement> -> <Print>\n";
        Print();
    }
    else if (token.lexeme == "scan") {
        if (debug) outputFile << "<Statement> -> <Scan>\n";
        Scan();
    }
    else if (token.lexeme == "while") 
    {
        if (debug) outputFile << "<Statement> -> <While>\n";
        While();
    }
    else 
    {
        error("Unexpected token" + token.lexeme + "\n");
    }
}


void Compound() {
    if (debug) outputFile << "<Compound> -> { <Statement List> }\n"; 
    match("Separator");     // {
    StatementList();
    match("Separator");     // }
}


void Assign() 
{
    if (debug) outputFile << "<Assign> -> <Identifier> = <Expression> ;\n";
    
    match("Identifier");
    match("Operator");      // =
    Expression(); 
    match("Separator");     // ;
}


void If() {
    if (debug) outputFile << "<If> -> if ( <Condition> ) <Statement> <If'> endif\n";
    
    match("Keyword");       // if
    match("Separator");     // (
    Condition();
    match("Separator");     // )
    Statement();
    IfPrime();
    match("Keyword");       // endif
}

void IfPrime() {
    Token token = peek();
    if (token.lexeme == "else") {
        if (debug) outputFile << "<If'> -> else <Statement>\n";
        match("Keyword");
        Statement();
    }
    else {
        if (debug) outputFile << "<If'> -> <Empty>\n";
        Empty();
    }
}


void Return() {
    Token token = peek();
    if (token.lexeme == "return") {
        if (debug) outputFile << "<Return> -> return <Return'>\n";
        match("Keyword");
        ReturnPrime();
    }
}

void ReturnPrime() {
    Token token = peek();
    if (token.lexeme != ";") {
        if (debug) outputFile << "<Return'> -> <Expression> ;\n";
        Expression();
        match("Separator");
    }
    else {
        if (debug) outputFile << "<Return'> -> ;\n";
        match("Separator");
    }
}


void Print() {
    if (debug) outputFile << "<Print> -> print ( <Expression> );\n";
    match("Keyword");       // print
    match("Separator");     // (
    Expression();
    match("Separator");     // )
    match("Separator");     // ;
}


void Scan() {
    if (debug) outputFile << "<Scan> -> ( <IDs> );\n";
    match("Keyword");       // scan
    match("Separator");     // (
    IDS();
    match("Separator");     // )
    match("Separator");     // ;
}


void While() {
    if (debug) outputFile << "<While> -> while ( <Condition> ) <Statement> endwhile\n";
    match("Keyword");       // while
    match("Separator");     // (
    Condition();
    match("Separator");     // )
    Statement();
    match("Keyword");        // endwhile
}


void Condition() {
    if (debug) outputFile << "<Condition> -> <Expression> <Relop> <Expression>\n";
    Expression();
    Relop();
    Expression();
}


void Relop() {
    Token token = peek();
    if (token.lexeme == "==") {
        if (debug) outputFile << "<Relop> -> ==\n";
        match("Operator");
    }
    else if (token.lexeme == "!=") {
        if (debug) outputFile << "<Relop> -> !=\n";
        match("Operator");
    }
    else if (token.lexeme == ">") {
        if (debug) outputFile << "<Relop> -> >\n";
        match("Operator");
    }
    else if (token.lexeme == "<") {
        if (debug) outputFile << "<Relop> -> <\n";
        match("Operator");
    }
    else if (token.lexeme == "<=") {
        if (debug) outputFile << "<Relop> -> <=\n";
        match("Operator");
    }
    else if (token.lexeme == "=>") {
        if (debug) outputFile << "<Relop> -> =>\n";
        match("Operator");
    }
    else {
        error("Expected '==', '!=', '>', '<', '<=', or '=>' but found " + token.lexeme + "\n");
    }
}


void Expression() 
{
    if (debug) outputFile << "<Expression> -> <Term> <Expression Prime>\n";
    Term();
    ExpressionPrime();
}


void ExpressionPrime() 
{
    if (peek().lexeme == "+") {
        match("Operator"); // ✅ Print "+" BEFORE rule
        if (debug) outputFile << "<Expression Prime> -> + <Term> <Expression Prime>\n";
        Term();
        ExpressionPrime();
    } 
    else if (peek().lexeme == "-") {
        match("Operator");
        if (debug) outputFile << "<Expression Prime> -> - <Term> <Expression Prime>\n";
        Term();
        ExpressionPrime();
    }
    else {
        if (debug) outputFile << "<Expression Prime> -> <Empty>\n";
        Empty();
    }
}


void Term() 
{
    if (debug) outputFile << "<Term> -> <Factor> <Term Prime>\n";
    Factor();
    TermPrime();
}


void TermPrime() 
{
    if (peek().lexeme == "*") {
        if (debug) outputFile << "<Term'> -> * <Factor> <Term'>\n";
        match("Operator");
        Factor();
        TermPrime();
    }
    else if (peek().lexeme == "/") {
        if (debug) outputFile << "<Term'> -> / <Factor> <Term'>\n";
        match("Operator");
        Factor();
        TermPrime();
    }
    else {
        if (debug) outputFile << "<TermPrime> -> <Empty>\n";  // Since there are no more terms, print epsilon
        Empty();
    }
}


void Primary() {

    Token token = peek();
    if (token.type == "Identifier") {
        if (debug) outputFile << "<Primary> -> <Identifier> <Primary'>\n";
        match("Identifier");
        PrimaryPrime();
    }
    else if (token.type == "Integer") {
        if (debug) outputFile << "<Primary> -> <Integer>\n";
        match("Integer");
    }
    else if (token.type == "Real") {
        if (debug) outputFile << "<Primary> -> <Real>\n";
        match("Real");
    }
    else if (token.lexeme == "(") {
        if (debug) outputFile << "<Primary> -> ( <Expression> )\n";

        match("Separator");
        Expression();
        match("Separator");
    }
    else if (token.lexeme == "true") {
        if (debug) outputFile << "<Primary> -> true\n";
        match("Keyword");
    }
    else if (token.lexeme == "false") {
        if (debug) outputFile << "<Primary> -> false\n";
        match("Keyword");
    }
    
}

void PrimaryPrime() {
    Token token = peek();
    if (token.lexeme == "(") {
        if (debug) outputFile << "<Primary'> -> ( IDS )\n";
        match("Separator");
        IDS();
        match("Separator");
    }
    else {
        if (debug) outputFile << "<Primary'> -> <Empty>\n";
        Empty();
    }
}


void Factor() 
{
    Token token = peek();
    if (token.lexeme == "-") {
        if (debug) outputFile << "<Factor> -> - <Primary>\n";
        match("Operator");
        Primary();
    }
    else {
        if (debug) outputFile << "<Factor> -> <Primary>\n";
        Primary();
    }
    
}


void Empty() {
    if (debug) outputFile << "<Empty> -> ε\n";
}


void parse(vector<Token> tokensList, const string& fileName) 
{
    tokens = tokensList;
    currentTokenIndex = 0;
    outputFile.open("./testcases/syntax_results/" + fileName + "-syntax.txt");

    Rat25s();

    if (peek().type != "EOF") 
    {
        error("Unexpected tokens at the end of input.\n");
    }

    outputFile.close();
}
