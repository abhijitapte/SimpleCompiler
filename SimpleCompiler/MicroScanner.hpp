
#ifndef MICROSCANNER_HPP
#define MICROSCANNER_HPP

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
    
    typedef std::pair<TokenClass, std::string> TokenBuffer;
    
    MicroScanner(const std::string& inputFile);
    ~MicroScanner();
    
    TokenBuffer Scan();
    
private:
    
    static void CheckReserved(
                    const std::string&          inToken,
                    MicroScanner::TokenBuffer&  outTokenBuffer);
    
    std::string m_inputFile;
    
};

#endif /* MICROSCANNER_HPP */
