#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;

int main()
{
    string strWhile = "abcdefgh";
    string::size_type indexWhile = 0;
    while (indexWhile < strWhile.size())
    {
        strWhile[indexWhile] = 'X';
        indexWhile++;
    }
    cout << " while: " << strWhile << endl;
    string strFor = "abcdefgh";

    for (string::size_type index = 0; index < strFor.size(); index++)
    {
        strFor[index] = 'X';
    }
    cout << " for: " << strFor << endl;

    return 0;
}