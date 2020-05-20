#include <iostream>
#include <vector>
#include <algorithm>
#include <list>

using namespace std;

int main()
{
    vector<int> iv = {1, 1, 2, 3, 4,4,5};
    list<int> il;
    unique_copy(iv.cbegin(), iv.cend(), back_inserter(il));
    for_each(il.cbegin(), il.cend(), [](int n){cout<<n<<" ";});
    cout<<endl;
    return 0;
}