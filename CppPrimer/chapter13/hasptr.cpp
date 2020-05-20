#include <string>
#include <iostream>
#include <vector>
#include <algorithm>


using namespace std;

class HasPtr
{
public:
    HasPtr(const string &s = string()) : ps(new string(s)), i(0) {}
    HasPtr(const HasPtr& hp):ps(new string(*hp.ps)), i(hp.i){}
    // HasPtr& operator=(const HasPtr&);
    HasPtr& operator=(HasPtr);
    bool operator<(const HasPtr&);
    ~HasPtr(){delete ps;}
    friend void swap(HasPtr&, HasPtr&);
    string getps(){return string(*ps);}
private:
    string *ps;
    int i;
};

// HasPtr& HasPtr::operator=(const HasPtr &rhp)
// {
//     //先创建一个拷贝.不可直接删除，如果rhp和this是同一个对象，ps就成为了无效指针
//     auto newp = new string(*rhp.ps);
//     //删除旧的
//         delete ps;
//         ps = newp;
//     i = rhp.i;
//     return *this;
// }
HasPtr& HasPtr::operator=(HasPtr rhp)
{
    swap(*this, rhp);
    return *this;
}
inline void swap(HasPtr &lhs, HasPtr &rhs)
{
    cout << "using swap!"<<endl;
    using std::swap;
    swap(lhs.ps, rhs.ps);
    swap(lhs.i, rhs.i);
}

bool HasPtr::operator<(const HasPtr &rhs)
{
    return *ps < *rhs.ps;
}

int main()
{
    vector<HasPtr> hpv;
    hpv.push_back(HasPtr("cef"));
    hpv.push_back(HasPtr("ace"));
    hpv.push_back(HasPtr("bds"));
    cout<<"cef,abe,bds" <<endl;
    sort(hpv.begin(), hpv.end());
    for(auto hp : hpv)
    {
        cout<<hp.getps()<<endl;
    }
}