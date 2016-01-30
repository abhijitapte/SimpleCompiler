
#ifndef MICROPARSER_HPP
#define MICROPARSER_HPP

#include <iostream>
#include <memory>

#include "MicroScanner.hpp"

/*
 * Extended CFG Defining Micro -
 *
 *  <program>           -> begin <statement list> end
 *  <statement list>    -> <statement> { <statement list> }
 *  <statement>         -> ID := <expression> ;
 *  <statement>         -> read ( <id list> ) ;
 *  <statement>         -> write ( <expr list> ) ;
 *  <id list>           -> ID {, ID}
 *  <expr list>         -> <expression> {, <expression>}
 *  <expression>        -> <primary> { <add op> <primary> }
 *  <primary>           -> ( <expression> )
 *  <primary>           -> ID
 *  <primary>           -> INTLITERAL
 *  <add op>            -> PLUSOP
 *  <add op>            -> MINUSOP
 *  <system goal>       -> <program> SCANEOF
 *
 */

class MicroParser {

public:
    
    MicroParser(const std::string& inputFile);
    ~MicroParser();
    
    void Parse();
    
private:
    
    /* 
     * Recursive Descent Parsing -
     * Each non-terminal has an associated parsing procedure.
     */
    
    // Matching routines for non-terminal symbols
    void SystemGoal();
    void Program();
    void StatementList();
    void Statement();
    void IDList();
    void Expression();
    void ExpressionList();
    void AddOp();
    void Primary();
    
    // Matching routine for terminal symbols
    void Match(MicroScanner::TokenClass);
    
    MicroScanner m_scanner;
    
    MicroScanner::Token GetNextToken();
    
    std::shared_ptr<MicroScanner::Token> m_currentToken;
};

#endif /* MICROPARSER_HPP */
