#include <iostream>
#include <vector>
using std::vector, std::cout, std::endl;

int main()
{
    int i=1;
    vector<int> vi{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    for(auto it = vi.begin(); it != vi.end(); ++it)
    {
        *it = *it * 2;
    }
    cout<< "vector items:";
       for(auto it = vi.cbegin(); it != vi.cend(); ++it)
    {
        cout<<" [" << *it << "]";
    }
    cout << endl;
    return 0;
}
