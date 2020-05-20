#include <iostream>
#include <forward_list>
using std::forward_list;
using std::cout;
using std::endl;

void remove_odd(forward_list<int> &ifl)
{
    auto curr = ifl.begin();
    auto prev = ifl.before_begin();
    while(curr != ifl.end())
    {
        if(*curr%2)
        {
            curr = ifl.erase_after(prev);
        }
        else
        {
            prev = curr;
            ++curr;
        }   
    }

}

void display(const forward_list<int> &ifl)
{
    cout<<"items of forward_list: ";
    for(auto cbeg = ifl.cbegin(); cbeg != ifl.cend(); ++cbeg)
    {
        cout<<*cbeg<<", ";
    }
    cout<<endl;
}

int main()
{
    forward_list<int> ifl = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    display(ifl);
    remove_odd(ifl);
    display(ifl);
    return 0;
}