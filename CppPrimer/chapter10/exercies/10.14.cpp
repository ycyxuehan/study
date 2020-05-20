#include <iostream>

int main()
{
    auto add = [](int a, int b){return a + b;};
    int a = 1;
    auto add_a = [a](int b){return a + b;};
    auto l = [a]()mutable ->bool{
        if(a==0)
        {
            return true;
        }
        --a;
        return false;
    };
}