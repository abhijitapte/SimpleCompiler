
#include "MicroParser.hpp"

MicroParser::MicroParser(const std::string& inputFile)
: m_scanner(inputFile)
{
    
}

MicroParser::~MicroParser()
{
    
}

void MicroParser::Parse()
{
    SystemGoal();
}

void MicroParser::SystemGoal()
{
    /* <system goal> ::= <program SCANEOF */
    Program();
    Match(MicroScanner::SCANEOF);
}

void MicroParser::Program()
{
    Match(MicroScanner::BEGIN);
    StatementList();
    Match(MicroScanner::END);
}

void MicroParser::StatementList()
{
    /*
     * <statement list> ::= <statement> { <statement> }
     */
    Statement();
    while (true)
    {
        MicroScanner::Token token = GetNextToken();
        MicroScanner::TokenClass tokenClass = token.first;
        
        switch(tokenClass)
        {
            /* 
             * List the same tokens that are in the productions
             * for the non-terminal symbol <statement>
             */
            case MicroScanner::ID:
            case MicroScanner::READ:
            case MicroScanner::WRITE:
                Statement();
                break;
            default:
                return;
        }
    }
}

void MicroParser::Statement()
{
    MicroScanner::Token token = GetNextToken();
    MicroScanner::TokenClass tokenClass = token.first;
    
    switch(tokenClass)
    {
        case MicroScanner::ID:
            /* <statement> ::= ID := <expression> ; */
            Match(MicroScanner::ID);
            Match(MicroScanner::ASSIGNOP);
            Expression();
            Match(MicroScanner::SEMICOLON);
            break;
            
        case MicroScanner::READ:
            /* <statement> ::= READ ( <id list> ) ; */
            Match(MicroScanner::READ);
            Match(MicroScanner::LPAREN);
            IDList();
            Match(MicroScanner::RPAREN);
            Match(MicroScanner::SEMICOLON);
            break;
            
        case MicroScanner::WRITE:
            /* <statement> ::= WRITE ( <id list> ) ; */
            Match(MicroScanner::WRITE);
            Match(MicroScanner::LPAREN);
            ExpressionList();
            Match(MicroScanner::RPAREN);
            Match(MicroScanner::SEMICOLON);
            break;
            
        default:
            std::cout << "Syntax error!" << std::endl;
            break;
    }
}

void MicroParser::IDList()
{
    /* <id list> ::= ID {, ID} */
    Match(MicroScanner::ID);
    
    MicroScanner::Token token;
    MicroScanner::TokenClass tokenClass;

    for (token = GetNextToken(), tokenClass = token.first;
         tokenClass == MicroScanner::COMMA;
         token = GetNextToken(), tokenClass = token.first)
    {
        Match(MicroScanner::COMMA);
        Match(MicroScanner::ID);
    }
}

void MicroParser::Expression()
{
    /*
     * <expression> ::= <primary> { <add op> <primary> }
     */
    Primary();
    MicroScanner::Token token;
    MicroScanner::TokenClass tokenClass;
    
    for (token = GetNextToken(), tokenClass = token.first;
         tokenClass == MicroScanner::PLUSOP || tokenClass == MicroScanner::MINUSOP;
         token = GetNextToken(), tokenClass = token.first)
    {
        AddOp();
        Primary();
    }
}

void MicroParser::ExpressionList()
{
    /*
     * <expr list> ::= <expression> {, <expression> }
     */
    
    Expression();
    
    MicroScanner::Token token;
    MicroScanner::TokenClass tokenClass;
    
    for (token = GetNextToken(), tokenClass = token.first;
         tokenClass == MicroScanner::COMMA;
         token = GetNextToken(), tokenClass = token.first)
    {
        Match(MicroScanner::COMMA);
        Expression();
    }
}

void MicroParser::AddOp()
{
    MicroScanner::Token token = GetNextToken();
    MicroScanner::TokenClass tokenClass = token.first;
    /* <add op> ::= PLUSOP | MINUSOP */
    if (tokenClass == MicroScanner::PLUSOP ||
        tokenClass == MicroScanner::MINUSOP)
        Match(tokenClass);
    else
    {
        std::cout << "Syntax error!" << std::endl;
    }
}

void MicroParser::Primary()
{
    MicroScanner::Token token = GetNextToken();
    MicroScanner::TokenClass tokenClass = token.first;
    
    switch(tokenClass)
    {
        case MicroScanner::LPAREN:
            /* <primary> ::= ( <expression> ) */
            Match(MicroScanner::LPAREN);
            Expression();
            Match(MicroScanner::RPAREN);
            break;
            
        case MicroScanner::ID:
            /* <primary> ::= ID */
            Match(MicroScanner::ID);
            break;
            
        case MicroScanner::INTLITERAL:
            /* <primary> ::= INTLITERAL */
            Match(MicroScanner::INTLITERAL);
            break;
            
        default:
            std::cout << "Syntax error!" << std::endl;
            break;
    }
}

MicroScanner::Token MicroParser::GetNextToken()
{
    MicroScanner::Token token;
    MicroScanner::TokenClass tokenClass;

    if (m_currentToken)
        token = *m_currentToken;
    else
    {
        token = m_scanner.GetToken();
        m_currentToken = std::make_shared<MicroScanner::Token>(token);
    }
    
    return token;
}

void MicroParser::Match(MicroScanner::TokenClass toMatch)
{
    MicroScanner::Token token;
    
    if (m_currentToken)
    {
        token = *m_currentToken;
        m_currentToken.reset();
    }
    else
        token = m_scanner.GetToken();
    
    MicroScanner::TokenClass tokenClass = token.first;
    
    if (tokenClass == toMatch)
    {
        /* All went correct */
        const std::string& tokenValue = token.second;
        std::cout << "Matched \"" << tokenValue << "\"" << std::endl;
    }
    else
    {
        std::cout << "Syntax error!" << std::endl;
    }
}
