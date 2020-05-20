#include <string>
#include <vector>
#include <memory>
using namespace std;

class StrBlob
{
public:
    typedef vector<string>::size_type size_type;
    StrBlob():data(make_shared<vector<string>>()){};
    StrBlob(initializer_list<string> il):data(make_shared<vector<string>>(il)){};
    size_type size() const { return data->size(); }
    bool empty() const { return data->empty(); }
    void push_back(const string &s) { data->push_back(s); }
    void pop_back();
    string &front();
    string &back();
    string& front() const;
    string& back() const;
private:
    shared_ptr<vector<string>> data;
    void check(size_type i, const string &msg) const;
};

void StrBlob::check(StrBlob::size_type i, const string &msg)const
{
    if(i>=data->size())
    {
        throw out_of_range(msg);
    }
}

string& StrBlob::front()
{
    check(0, "front on empty strblob");
    return data->front();
}

string& StrBlob::back()
{
    check(0, "back on empty strblob");
    return data->back();
}

void StrBlob::pop_back()
{
    check(0, "pop_back on empty strblob");
    data->pop_back();
}

string& StrBlob::front() const
{
    check(0, "front on empty strblob");
    return data->front();
}

string& StrBlob::back() const
{
    check(0, "back on empty strblob");
    return data->back();
}