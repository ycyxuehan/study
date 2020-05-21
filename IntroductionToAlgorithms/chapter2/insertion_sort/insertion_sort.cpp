#include <iostream>
#include <vector>
#include <string>

using namespace std;

template <typename _T, size_t _ST>
void insertion_sort(_T (&t)[_ST])
{
    for(size_t i = 1; i < _ST; ++i)
    {
        auto item = t[i];
        int j = i -1;
        while (j >=0 && t[j] > item)
        {
            t[j+1] = t[j];
            --j;
        }
        t[j+1] = item;
    }
}

int main()
{
    int ia[] = {3, 1, 5 , 9, 2, 8};
    // string ia[] = {"c", "d", "a", "w", "f", "e"};
    cout<<"source: ";
    for(auto i : ia)
    {
        cout << i <<" ";
    }
    cout<<endl;
    insertion_sort(ia);
    cout<<"sorted: ";
    for(auto i : ia)
    {
        cout << i <<" ";
    }
    cout<<endl;
    return 0;
}