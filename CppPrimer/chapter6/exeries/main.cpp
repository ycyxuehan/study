#include <iostream>
#include "6.7.h"
#include "6.10.h"
using std::cout;
using std::endl;

int main()
{
    cout<<"1:"<<isUsed()<<endl;
    cout<<"2:"<<isUsed()<<endl;
    cout<<"3:"<<isUsed()<<endl;
    int x = 1, y = 2;
    swap(&x, &y);
    cout<<"point swap, x:"<<x<<",y:"<<y<<endl;
    swap(x, y);
    cout<<"refrence swap, x:"<<x<<",y:"<<y<<endl;
    return 0;
}
 
