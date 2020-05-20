#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::endl;
using std::vector;
using std::string;

void display(vector<string>::const_iterator cbeg, vector<string>::const_iterator cend)
{
    if(cbeg != cend)
    {
        cout << *cbeg << endl;
        display(++cbeg, cend);
    }
}

int main()
{
    vector<string> vs{"a", "b"};
    display(vs.cbegin(), vs.cend());
}