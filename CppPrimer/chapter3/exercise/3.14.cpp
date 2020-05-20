#include <iostream>
#include <vector>
using std::cin, std::cout, std::endl, std::vector;

int main()
{
    cout << "input some numbers:"<<endl;
    vector<int> vi;
    int num;
    while (cin>>num)
    {
        vi.push_back(num);
    }
    cout << "vector: " << vi.size() <<endl;
    return 0;
}