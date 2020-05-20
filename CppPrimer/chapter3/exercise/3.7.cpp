#include <iostream>
#include <string>
using std::cout;
using std::string;
using std::endl;

int main()
{
    string str("abcdefgh");
    for (char &c : str)
    {
        c = 'X';
    }
    cout << str <<endl;
    return 0;
}