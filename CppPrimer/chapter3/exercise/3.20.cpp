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
    for(int i=0; i<vi.size(); i+=2){
        if(i+1>= vi.size())
        {
            cout<< vi[i] <<endl;
            break;
        }
        auto sum = vi[i] + vi[i+1];
        cout<< sum <<endl;
        
    }
    for(int i=0, j=vi.size()-1; i <=j; i++, j--)
    {
        if(i==j)
        {
            cout<< vi[i] <<endl;
            break;
        }
        cout<<vi[i] + vi[j] << endl;

    }
    return 0;
}