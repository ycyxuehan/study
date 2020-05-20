#include <iostream>
using std::cin;
using std::cout;
using std::endl;

int main()
{
    int x = 0, y = 0, z = 0;
    cout << "please input two numbers:";
    cin >> x;
    while (cin >> y)
    {
        try
        {
            if (y == 0)
            {
                throw(std::runtime_error("y == 0"));
            }
            z = x / y;
            cout << x << "/" << y << " = " << z << endl;
        }
        catch (std::runtime_error e)
        {
            // std::cerr << e.what() << '\n';
            cout << "y must not 0, reinput y or input 'n' to exit" << endl;
        }
    }

    return 0;
}