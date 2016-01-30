
#ifndef MICROSCANNER_HPP
#define MICROSCANNER_HPP

#include <fstream>

class MicroScanner {
    
public:
    
    typedef enum {
        BEGIN,
        END,
        READ,
        WRITE,
        ID,
        INTLITERAL,
        LPAREN,
        RPAREN,
        SEMICOLON,
        COMMA,
        ASSIGNOP,
        PLUSOP,
        MINUSOP,
        SCANEOF
    } TokenClass;
    
    typedef std::pair<TokenClass, std::string> Token;
    
    MicroScanner(const std::string& inputFile);
    ~MicroScanner();
    
    Token GetToken();
    
private:
    
    Token Scan();
    
    
    static void CheckReserved(
                    const std::string&    inTokenBuffer,
                    MicroScanner::Token&  outToken);
    
    std::ifstream m_inputFileStream;
    
};

#endif /* MICROSCANNER_HPP */
