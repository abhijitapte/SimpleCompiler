
#include <iostream>
#include <fstream>
#include <map>

#include "MicroScanner.hpp"

MicroScanner::MicroScanner(const std::string& inputFile)
: m_inputFile(inputFile)
{
    
}

MicroScanner::~MicroScanner()
{
    
}

/*static*/ void MicroScanner::CheckReserved(
                                const std::string&          inToken,
                                MicroScanner::TokenBuffer&  outTokenBuffer)
{
    typedef std::map<std::string, TokenClass> ReservedTokens;
    static ReservedTokens reservedTokens = {
        std::make_pair("begin", BEGIN),
        std::make_pair("end", END),
        std::make_pair("read", READ),
        std::make_pair("write", WRITE)
    };
    
    auto reservedIter = reservedTokens.find(inToken);
    
    MicroScanner::TokenClass tokenClass = (reservedIter != reservedTokens.end())?
    reservedIter->second : ID;
    outTokenBuffer = std::make_pair(tokenClass, inToken);
}

MicroScanner::TokenBuffer MicroScanner::Scan()
{
    TokenBuffer tokenBuffer;
    std::string token;
    
    std::ifstream::char_type inChar;
    std::ifstream::char_type c;

    std::ifstream is (m_inputFile, std::ifstream::binary);
    
    if (is.eof())
    {
        return std::make_pair(SCANEOF, "");
    }
    
    while (is.get(inChar))
    {
        TokenClass tokenClass;
        switch(inChar)
        {
            case '-':
                is.get(c);
                if (c == '-')
                {
                    /* skip one line comments (which start with --) */
                    do
                        is.get(inChar);
                    while (inChar != '\n');
                    break;
                }
                else
                    tokenClass = MINUSOP;
            case '(':
                tokenClass = LPAREN;
            case ')':
                tokenClass = RPAREN;
            case ';':
                tokenClass = SEMICOLON;
            case ',':
                tokenClass = COMMA;
            case '+':
                tokenClass = PLUSOP;
                token += inChar;
                return std::make_pair(tokenClass, token);
            case ':':
                token += inChar;
                /* looking for ":=" */
                is.get(c);
                if (c == '=')
                {
                    token += inChar;
                    return std::make_pair(ASSIGNOP, token);
                }
                else
                {
                    is.unget();
                    /* lexical_error(); */
                }
                break;
            
            default:
                if (std::isspace(inChar))
                    continue;
                else if (std::isalpha(inChar))
                {
                    /*
                     * ID ::= LETTER | ID LETTER
                     *               | ID DIGIT
                     *               | ID UNDERSCORE
                     */
                    token += inChar;
                    for (is.get(c); std::isalnum(c) || c == '_'; is.get(c))
                        token += c;
                    is.unget();
                    CheckReserved(token, tokenBuffer);
                    return tokenBuffer;
                }
                else if (std::isdigit(inChar))
                {
                    /*
                     * INTLITERAL ::= DIGIT |
                     *                INTLITERAL DIGIT
                     */
                    token += inChar;
                    for (is.get(c); std::isdigit(c); is.get(c))
                        token += c;
                    is.unget();
                    tokenBuffer = std::make_pair(INTLITERAL, token);
                    return tokenBuffer;
                }
                else
                {
                    /* lexical_error */
                }
                break;
        }
    }
    
    return std::make_pair(SCANEOF, "");
}
