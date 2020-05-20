#include <iostream>

int main()
{
    int sum = 1, val = 1;
    while (val <= 10)
    {
        /* code */
        sum += val;
        ++val;
    }

    std::cout << "Sum of 1 to 10 inclusive is "
                << sum << std::endl;
    return 0;
}