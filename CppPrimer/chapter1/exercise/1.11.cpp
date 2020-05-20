#include <iostream>
using namespace std;

int main()
{
    cout << "Please input two numbers" << endl;
    int v1 = 0, v2 = 0;
    cin >> v1 >> v2;
    int sum = 0, max = 0, min = 0;
    v1 > v2 ? max = v1, min = v2, v1 = v2 : max = v2, min = v1 ;
    while (v1 <= max)
    {
        /* code */
        sum += v1;
        ++v1;
    }
    cout << "Sum of " << min << " to " << max << " inclusive is " << sum << endl; 

    return 0;
}