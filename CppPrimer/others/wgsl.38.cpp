#include <iostream>
#include <string>

using namespace std;

string display(int n)
{
    string result = "1";
    for (int i = 1; i < n; ++i)
    {
        char curr = result[0];
        string res = "";
        int count = 0;
        for (auto s : result)
        {
            if (s == curr)
            {
                ++count;
            }
            else
            {
                res += to_string(count); // + to_string(curr);
                res.push_back(curr);
                count = 1;
                curr = s;
            }
        }
        res += to_string(count);
        res.push_back(curr);
        result = res;
    }
    return result;
}
int main()
{
    cout << display(1) << endl;
    cout << display(2) << endl;
    cout << display(3) << endl;
    cout << display(4) << endl;
    cout << display(5) << endl;
    cout << display(6) << endl;
    cout << display(7) << endl;

    return 0;
}