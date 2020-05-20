#include <iostream>
#include <vector>
using std::vector;
using std::cout;
using std::cin;
using std::endl;

int main()
{
    vector<int> vi(11, 0);
    auto begin = vi.begin();
    unsigned grade;
    while (cin>>grade)
    {
        /* code */
        if(grade <= 100)
        {
            int offset = grade/10;
            auto it = begin + offset;
            ++*it;
        }
    }
    for(;begin != vi.end(); ++begin)
    {
        cout<<" "<<*begin;
    }
    cout<<endl;
    return 0;
}
