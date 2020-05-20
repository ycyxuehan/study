#include "stdfx.h"

template<class Tp, size_t Ind> Tp* beg(Tp (&tp)[Ind])
{
    return tp;
}
template<class Tp, size_t Ind> Tp* en(Tp (&tp)[Ind])
{
    return tp + Ind;
}

int main()
{
    int ia[1];
    auto b = beg(ia);
    auto be = begin(ia);
    auto end1 = end(ia);
    auto e = en(ia);
    cout<<b <<" "<< be<<endl;
    cout<<e <<" "<< end1<<endl;
    return 0;
}