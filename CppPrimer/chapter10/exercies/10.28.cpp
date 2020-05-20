#include <iostream>
#include <vector>
#include <list>
#include <algorithm>

using namespace std;

int main()
{
    vector<int> iv = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    // 123456789
    vector<int> ins;
    //12345678
    vector<int> bins;
    //987654321
    list<int> fins;
    copy(iv.cbegin(), iv.cend(), inserter(ins, ins.begin()));
    copy(iv.cbegin(), iv.cend(), back_inserter(bins));
    copy(iv.cbegin(), iv.cend(), front_inserter(fins)); 
    for_each(ins.cbegin(), ins.cend(), [](int n){cout<<n<<" ";});
    cout<<endl;
    for_each(bins.cbegin(), bins.cend(), [](int n){cout<<n<<" ";});
    cout<<endl;
    for_each(fins.cbegin(), fins.cend(), [](int n){cout<<n<<" ";});
    cout<<endl;

}