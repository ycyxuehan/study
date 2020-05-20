#include "stdfx.h"

template<typename T> void print(const T &t)
{
    auto len = sizeof(t);
    if(len == 0)
        return;
    for(int i=0; i<len/sizeof(t[0]); ++i){
        cout<<t[i];
    }
}

int main()
{
    int ia[] = {1, 2, 3, 4, 5};
    // cout<<sizeof(ia);
    print(ia);
    return 0;
}