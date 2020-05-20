#include <iostream>

using namespace std;

int main()
{

    int sum = 0, val = 10;
    while (val >0)
    {
        /* code */
        sum += val;
        --val;
    }

    std::cout << "Sum of 10 to 1 inclusive is "
              << sum << std::endl;
    return 0;
}