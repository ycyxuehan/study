#include <iostream>
#include <deque>
#include <list>

using std::cout;
using std::endl;
using std::list;
using std::deque;

void odd_even(const list<int> &l, deque<int> &odd, deque<int> &even)
{
    for(auto cbeg = l.cbegin(); cbeg != l.cend(); ++cbeg)
    {
        if(*cbeg % 2 == 0)
        {
            even.push_back(*cbeg);
        }
        else
        {
            odd.push_back(*cbeg);
        }
        
    }
}

void dislay(const deque<int> &d)
{
    cout << "items of deque: ";
    for(auto cbeg = d.cbegin(); cbeg != d.cend(); ++cbeg)
    {
        cout  << *cbeg << " , " ;
    }
    cout << endl;
}

void dislay(const list<int> &l)
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
    list<int> l = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    deque<int> odd;
    deque<int> even;
    odd_even(l, odd, even);
    dislay(l);
    dislay(odd);
    dislay(even);
    return 0;
}