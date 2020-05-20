#include <iostream>
#include <vector>
#include <list>

using std::cout;
using std::endl;
using std::list;
using std::vector;

void copy(const int ia[], int size, vector<int> &iv)
{
    iv.clear();
    for (int i = 0; i < size; ++i)
    {
        iv.insert(iv.end(), ia[i]);
    }
}

void copy(const int ia[], int size, list<int> &il)
{
    il.clear();
    for (int i = 0; i < size; ++i)
    {
        il.push_back(ia[i]);
    }
}

void dislay(const vector<int> &d)
{
    cout << "items of vector: ";
    for (auto cbeg = d.cbegin(); cbeg != d.cend(); ++cbeg)
    {
        cout << *cbeg << " , ";
    }
    cout << endl;
}

void dislay(const list<int> &l)
{
    cout << "items of list: ";
    for (auto cbeg = l.cbegin(); cbeg != l.cend(); ++cbeg)
    {
        cout << *cbeg << " , ";
    }
    cout << endl;
}

void erase(list<int> &il, bool odd)
{
    auto iter = il.begin();
    while (iter != il.end())
    {
        if (int(odd) == *iter % 2)
        {
            //数字需要移除
            iter = il.erase(iter);
        }
        else
        {
            //数字不需要移除
            ++iter;
        }
    }
}

void erase(vector<int> &iv, bool odd)
{
    auto iter = iv.begin();
    while (iter != iv.end())
    {
        if (int(odd) == *iter % 2)
        {
            //数字需要移除
            iter = iv.erase(iter);
        }
        else
        {
            //数字不需要移除
            ++iter;
        }
    }
}

int main()
{
    int ia[] = {0, 1, 1, 2, 3, 5, 8, 13, 21, 55, 89};
    vector<int> iv;
    list<int> il;
    copy(ia, sizeof(ia)/sizeof(ia[0]), iv);
    dislay(iv);
    erase(iv, false);
    dislay(iv);

    copy(ia, sizeof(ia)/sizeof(ia[0]), il);
    dislay(il);
    erase(il, true);
    dislay(il);
    return 0;
}