#include <iostream>
#include <deque>
#include <list>
#include <string>

using std::string;
using std::list;
using std::cin;
using std::cout;
using std::deque;
using std::endl;

void init(deque<string> &d)
{
    string s;
    while (cin>>s && s!="q")
    {
        d.push_back(s);
    }
    
}

void init(list<string> &l)
{
    string s;
    while (cin>>s && s!="q")
    {
        l.push_back(s);
    }
}

void dislay(const deque<string> &d)
{
    cout << "items of deque: ";
    for(auto cbeg = d.cbegin(); cbeg != d.cend(); ++cbeg)
    {
        cout  << *cbeg << " , " ;
    }
    cout << endl;
}

void dislay(const list<string> &l)
{
    cout << "items of list: ";
    for(auto cbeg = l.cbegin(); cbeg != l.cend(); ++cbeg)
    {
        cout << *cbeg << " , ";
    }
    cout <<endl;
}

int main()
{
    deque<string> d;
    list<string> l;
    init(d);
    dislay(d);
    init(l);
    dislay(l);
    return 0;
}