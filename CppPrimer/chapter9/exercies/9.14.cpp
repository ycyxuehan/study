#include <vector>
#include <list>
#include <string>
#include <iostream>

using std::vector;
using std::list;
using std::string;
using std::cout;
using std::endl;

void l2v(list<char *> l, vector<string> &v)
{
    v.assign(l.cbegin(), l.cend());
}

int main()
{
    list<char *> l = {"aaa", "bbb", "ccc"};
    vector<string> v;
    l2v(l, v);
    for(auto s :v)
    {
        cout<<s<<endl;
    }
}