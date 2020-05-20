#include <iostream>
#include <vector>
#include <string>
using std::vector;
using std::string;
using std::cout, std::endl;

void display(vector<int> v, string name)
{
    auto size = v.size();
    cout<<"vector " << name <<" size: "<<size <<" values:";
    for(auto vi : v)
    {
        cout<<" " << vi <<"]";
    }
    cout << endl;
}

void display(vector<string> v, string name)
{
    auto size = v.size();
    cout<<"vector " << name <<" size: "<<size <<" values:";
    for(auto vi : v)
    {
        cout<<" [" << vi <<"]";
    }
    cout << endl;
}

int main()
{
    vector<int> v1;
    vector<int> v2(10);
    vector<int> v3(10, 42);
    vector<int> v4{10};
    vector<int> v5{10, 42};
    vector<string> v6{10};
    vector<string> v7{10, "hi"};
    display(v1, "v1");
    display(v2, "v2");
    display(v3, "v3");
    display(v4, "v4");
    display(v5, "v5");
    display(v6, "v6");
    display(v7, "v7");

    return 0;
}