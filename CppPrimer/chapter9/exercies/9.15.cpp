#include <iostream>
#include <vector>
#include <list>
using std::cout;
using std::endl;
using std::list;
using std::vector;

bool equal(vector<int> vi1, vector<int> vi2)
{
    return vi1 == vi2;
}

bool equal(list<int> li1, list<int> li2)
{
    return li1 == li2;
}

bool equal(vector<int> vi, list<int> li)
{
    if (vi.size() != li.size())
    {
        return false;
    }
    auto vcbeg = vi.cbegin();
    auto lcbeg = li.cbegin();

    for (; vcbeg != vi.cend() && lcbeg != li.cend(); ++vcbeg, ++lcbeg)
    {
        if (*vcbeg != *lcbeg)
        {
            return false;
        }
    }
    return true;
}

int main()
{
    vector<int> vi1 = {1, 2, 3, 4, 5};
    vector<int> vi2 = {2, 3, 4, 5, 6};
    vector<int> vi3 = {2, 3, 4};
    list<int> li1 = {2, 3, 4};
    list<int> li2 = {2, 3, 4};

    cout << "vi1 == vi2: " << equal(vi1, vi2) << endl;
    cout << "li1 == li2: " << equal(li1, li2) << endl;
    cout << "vi1 == li1: " << equal(vi1, li2) << endl;
    cout << "vi3 == li1: " << equal(vi3, li1) << endl;

    return 0;
}