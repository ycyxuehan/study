#include <iostream>
#include <vector>
#include <random>

using namespace std;

void generate(vector<int> &iv, size_t length)
{
    default_random_engine e;
    for(int i=0; i<length; ++i)
    {
        iv.push_back(e());
    }
}

void selection_sort(vector<int> &iv)
{
    for(auto b = iv.begin(); b != iv.end(); ++b)
    {
        auto beg = b;
        auto curr = b;
        while (beg != iv.end())
        {
            if(*beg < *curr)
            {
                curr = beg;
            }
            ++beg;
        }
        if(curr != b)
        {
            auto tmp = *b;
            *b = *curr;
            *curr = tmp;
        }
    }
}

int main()
{
    vector<int> iv;
    generate(iv, 10);
    cout<<"source: ";
    for(auto i : iv)
    {
        cout<<i<<" ";
    }
    cout<<endl;
    selection_sort(iv);
    cout<<"sorted: ";
    for(auto i : iv)
    {
        cout<<i<<" ";
    }
    cout<<endl;    
    return 0;
}