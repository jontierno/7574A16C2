#include <sstream>
#include <string>
#include <sstream>
#include <iostream>
//#define ITOS( x ) static_cast< std::ostringstream & >(( std::ostringstream() << std::dec << x )).str()
std::string ITOS(int value) {

    std::ostringstream temp;
    temp<<value;
    std::string ts =temp.str();
    std::cout<< value << " " << ts << std::endl;
    return temp.str();
}