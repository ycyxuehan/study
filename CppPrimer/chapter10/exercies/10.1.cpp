#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <list>
using std::count;
using std::cout;
using std::list;
using std::string;
using std::vector;

int item_count(const vector<int> &iv, int item)
{
    return count(iv.cbegin(), iv.cend(), item);
}

int item_count(const list<string> &sv, string item)
{
    return count(sv.cbegin(), sv.cend(), item);
}

int main()
{
    vector<int> iv = {1, 2, 3, 4, 5, 6, 7, 8, 9, 3, 5, 9, 4, 3};
    list<string> sv = {"a", "b", "c", "a", "c", "d", "a"};
    cout << "count 1: " << item_count(iv, 1);
    cout << "\ncount 'a':" << item_count(sv, "a") << std::endl;
    return 0;
}