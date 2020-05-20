#include <iostream>

using namespace std;

int main()
{

    int sum = 0, val = 50;
    while (val <= 100)
    {
        /* code */
        sum += val;
        ++val;
    }

    std::cout << "Sum of 1 to 10 inclusive is "
              << sum << std::endl;
    return 0;
}