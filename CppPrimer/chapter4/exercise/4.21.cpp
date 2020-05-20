#include <iostream>
#include <vector>
using std::vector;
using std::cout;
using std::endl;

int main()
{
    vector<int> vi{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto it = vi.begin();
    while (it != vi.end())
    {
        *it = *it %2 ==1?*it*2:*it;
        cout<<*it<<" ";
        it++;
    }
    cout<<endl;
    return 0;
}