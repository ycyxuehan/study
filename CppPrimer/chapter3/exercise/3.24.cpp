#include <iostream>
#include <vector>
using std::cin, std::cout, std::vector, std::endl;

int main()
{
    vector<int> vi;
    int num;
    while (cin>>num)
    {
        vi.push_back(num);
    }
    for(auto it = vi.cbegin(); it != vi.cend(); ++it){
        auto sum = *it + *(it +1);
        cout<< sum <<endl;
        
    }
    cout<<"====="<<endl;
    for(auto it1 = vi.cbegin(), it2 = vi.cend() - 1; it1 <=it2; it1++, --it2)
    {
        if(it1==it2)
        {
            cout<<"only one number: "<< *it1 <<endl;
            break;
        }
        cout<<*it1 + *it2 << endl;

    }
    return 0;
}