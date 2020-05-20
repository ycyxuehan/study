#include <iostream>
using std::cout;
using std::endl;

int main()
{
    int i = 1, j=5, k=6;
    auto res = i!=j<k;
    cout<<res<<endl;
    return 0;
}