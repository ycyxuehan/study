#include <vector>
#include <string>
#include <memory>

using namespace std;

template <typename T>
class Blob
{
public:
    typedef T value_type;
    typedef typename vector<T>::size_type size_type;
    Blob();
    Blob(initializer_list<T> il);
    size_type size() const { return data->size(); }
    bool empty() const { return data->empty(); }
    void push_back(const T &t) { data->push_back(t); }
    void push_back(T &&t) { data->push_back(std::move(t)); }
    void pop_back();
    T &back();
    T &operator[](size_type i);

private:
    shared_ptr<vector<T>> data;
    void check(size_type i, const string &msg) const;
};

template <typename T>
void Blob<T>::check(size_type i, const string &msg) const
{
    if (i >= data->size())
        throw out_of_range(msg);
}

template <typename T>
T &Blob<T>::back()
{
    check(0, "back on empty blob");
    return data->back();
}

template <typename T>
T &Blob<T>::operator[](size_type i)
{
    check(i, "subscript out of range");
    return (*data)[i];
}

template <typename T>
void Blob<T>::pop_back()
{
    check(0, "pop_back on empty blob");
    data->pop_back();
}

template <typename T>
Blob<T>::Blob() : data(make_shared<vector<T>>()) {}

template <typename T>
Blob<T>::Blob(initializer_list<T> il) : data(make_shared<vector<T>>(il)) {}

template <typename T>
class BlobPtr
{
public:
    BlobPtr()::curr(0) {}
    BlobPtr(Blob<T> &a, size_t sz = 0) : wptr(a.data), curr(sz) {}
    T &operator*() const
    {
        auto p = check(curr, "dereference past end");
        return (*p)[curr];
    }
    //处于模板类作用域时，可以直接使用BlobPtr&，而不需要提供实参BlobPtr<T>&

    BlobPtr &operator++();
    BlobPtr &operator--();
    BlobPtr operator++(int);

private:
    shared_ptr<vector<T>> check(size_t, const string &) const;
    weak_ptr<vector<T>> wptr;
    size_t curr;
};
//这里需要提供实参，已经离开作用域
template <typename T>
BlobPtr<T> BlobPtr<T>::operator++(int)
{
    BlobPtr ret = *this;
    ++*this;
    return ret;
}