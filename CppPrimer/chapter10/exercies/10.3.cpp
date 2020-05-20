#include <iostream>
#include <numeric>
#include <vector>

using std::vector;
using std::accumulate;
using std::cout;

int main()
{
    vector<int> iv = {1, 3, 4, 5 ,6};
    auto sum = accumulate(iv.cbegin(), iv.cend(), 0);
    cout<<"sum:" <<sum <<std::endl;
    vector<double> dv = {1.3, 2.5, 4.8, 0.6};
    auto dsum = accumulate(dv.cbegin(), dv.cend(), 0);
    cout<<"sum:" <<dsum <<std::endl;
    return 0;
}