#include <iostream>
#include <vector>
#include <string>
using std::cin, std::cout, std::vector, std::endl, std::string;

int main()
{
    vector<string> vs;
    for(string buffer; cin>>buffer /*&& buffer != "q!"*/;vs.push_back(buffer));
    return 0;
}