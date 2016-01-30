
#include <iostream>
#include <map>

#include "MicroScanner.hpp"

MicroScanner::MicroScanner(const std::string& inputFile)
: m_inputFileStream(inputFile, std::ifstream::binary)
{
}

MicroScanner::~MicroScanner()
{
    
}

/*static*/ void MicroScanner::CheckReserved(
                                const std::string&   inTokenBuffer,
                                MicroScanner::Token& outToken)
{
    typedef std::map<std::string, TokenClass> ReservedTokens;
    static ReservedTokens reservedTokens = {
        std::make_pair("begin", BEGIN),
        std::make_pair("end", END),
        std::make_pair("read", READ),
        std::make_pair("write", WRITE)
    };
    
    auto reservedIter = reservedTokens.find(inTokenBuffer);
    
    MicroScanner::TokenClass tokenClass = (reservedIter != reservedTokens.end())?
    reservedIter->second : ID;
    outToken = std::make_pair(tokenClass, inTokenBuffer);
}

MicroScanner::Token MicroScanner::GetToken()
{
    Token token;
    std::string tokenBuffer;
    
    std::ifstream::char_type inChar;
    std::ifstream::char_type c;
    
    std::ifstream& is = m_inputFileStream;
    
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
                {
                    tokenClass = MINUSOP;
                    tokenBuffer += inChar;
                }
                return std::make_pair(tokenClass, tokenBuffer);
            case '(':
                tokenClass = LPAREN;
                tokenBuffer += inChar;
                return std::make_pair(tokenClass, tokenBuffer);
            case ')':
                tokenClass = RPAREN;
                tokenBuffer += inChar;
                return std::make_pair(tokenClass, tokenBuffer);
            case ';':
                tokenClass = SEMICOLON;
                tokenBuffer += inChar;
                return std::make_pair(tokenClass, tokenBuffer);
            case ',':
                tokenClass = COMMA;
                tokenBuffer += inChar;
                return std::make_pair(tokenClass, tokenBuffer);
            case '+':
                tokenClass = PLUSOP;
                tokenBuffer += inChar;
                return std::make_pair(tokenClass, tokenBuffer);
            case ':':
                tokenBuffer += inChar;
                /* looking for ":=" */
                is.get(c);
                if (c == '=')
                {
                    tokenBuffer += c;
                    return std::make_pair(ASSIGNOP, tokenBuffer);
                }
                else
                {
                    is.unget();
                    /* lexical_error */
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
                    tokenBuffer += inChar;
                    for (is.get(c); std::isalnum(c) || c == '_'; is.get(c))
                        tokenBuffer += c;
                    is.unget();
                    CheckReserved(tokenBuffer, token);
                    return token;
                }
                else if (std::isdigit(inChar))
                {
                    /*
                     * INTLITERAL ::= DIGIT |
                     *                INTLITERAL DIGIT
                     */
                    tokenBuffer += inChar;
                    for (is.get(c); std::isdigit(c); is.get(c))
                        tokenBuffer += c;
                    is.unget();
                    token = std::make_pair(INTLITERAL, tokenBuffer);
                    return token;
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
