#include <iostream>
#include <sstream>

// #include <string>
using std::istream;
using std::istringstream;
using std::cout;

istream &read(istream &is)
{
    char c;
    auto oldState = is.rdstate();
    is.clear();
    while (is >>c && !is.eof())
    {
        cout << c;
    }
    is.setstate(oldState);
    return is;
}

int main()
{
    istringstream istr;
    istr.str("a b c d e\0");
    read(istr);
    return 0;
}