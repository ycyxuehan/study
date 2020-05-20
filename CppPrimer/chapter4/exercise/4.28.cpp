#include <iostream>
using std::endl;
using std::cout;

int main(){
    int n;
    float f;
    double d;
    bool b;
    char c;
    long l;
    cout << "int:    " << sizeof(n) <<endl;
    cout << "long:   " << sizeof(l) <<endl;
    cout << "float:  " << sizeof(f) <<endl;
    cout << "double: " << sizeof(d) <<endl;
    cout << "bool:   " << sizeof(b) <<endl;
    cout << "char:   " << sizeof(c) <<endl;
    int x[10];
    int *p = x;
    cout << sizeof(x)/sizeof(*x) <<endl;
    cout<<sizeof(p) <<" " << sizeof(p)/sizeof(*p) << endl;
}