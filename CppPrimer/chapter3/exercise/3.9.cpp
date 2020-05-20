#include <iostream>
#include <string>
using std::cout, std::cin, std::string, std::endl;

int main()
{
    cout << "input a string:" << endl;
    string inputStr;
    cin >> inputStr;
    string outputStr(inputStr.size() - 1, '?');
    string::size_type n = 0;
    for (auto c : inputStr)
    {
        if (!ispunct(c))
        {
            outputStr[n] = c;
            n++;
        }
    }
    cout << "new string: " << outputStr << endl;
    return 0;
}