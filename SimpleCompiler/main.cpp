
#include <iostream>

#include "MicroParser.hpp"

int main(int argc, const char * argv[])
{
    using std::cout;
    using std::endl;
    
    if (argc >= 2)
    {
        std::string inputFile = argv[1];
        MicroParser parser(inputFile);
        parser.Parse();
    }
    else
    {
        cout << "No input file" << endl;
        cout << "Usage: mcc <input-micro-file>" << endl;
    }
    return 0;
}
