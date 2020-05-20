#include <iostream>
#include <forward_list>
#include <string>
using std::string;
using std::forward_list;
using std::cout;
using std::endl;

void insert_string(forward_list<string> &sfl, string s1, string s2)
{
    auto curr = sfl.begin();
    auto prev = sfl.before_begin();
    bool inserted = false;
    while (curr != sfl.end())
    {
        if(*curr == s1)
        {
            curr = sfl.insert_after(curr, s2);
            // inserted = true;
            break;
        }
        ++curr;
        if(curr == sfl.end())
        {
            sfl.insert_after(prev, s2);
        }
        prev = curr;

    }
}

void display(const forward_list<string> &ifl)
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
    forward_list<string> sfl = {"a", "b", "c", "d"};
    display(sfl);
    insert_string(sfl, "b", "e");
    display(sfl);
    insert_string(sfl, "f", "f");
    display(sfl);
    return 0;
}