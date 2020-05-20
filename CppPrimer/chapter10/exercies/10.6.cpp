#include <iostream>
#include <vector>
#include <algorithm>

int main()
{
    int max = 10;
    std::vector<int> iv(max, 1);
    std::fill_n(iv.begin(), max, 0);
    std::cout<<"items of iv: ";
    for(auto cbeg = iv.cbegin(); cbeg != iv.cend(); ++cbeg)
    {
        std::cout<<*cbeg<<",";
    }
    std::cout<<std::endl;
}