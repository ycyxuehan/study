#include "stdfx.h";

template<class Iter, class Value> Iter find2(Iter cbeg, Iter cend, const Value &v)
{
    while (cbeg != cend)
    {
        if(*cbeg == v)
        {
            return cbeg;
        }
        ++ cbeg;
    }
    return cend;
}



int main()
{
    vector<int> iv = {1, 2, 3, 4};
    list<string> sl = {"a", "b", "c"};
    auto a = find2(iv.cbegin(), iv.cend(), 1);
    auto b = find2(sl.cbegin(), sl.cend(), "a");
    cout<<*a<<endl;
    cout<<*b<<endl;
    auto c = find(iv.cbegin(), iv.cend(), 1);
    
    return 0;
}