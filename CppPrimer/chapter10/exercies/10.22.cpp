#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include <functional>

using namespace std;
using std::placeholders::_1;
bool short_word(const string &s, size_t threshold)
{
    return s.size() <= threshold;
}
bool small_number(const int n, const string &threshold)
{
    return n > threshold.size();
}
void read(const string &line, vector<string> &sv)
{
    istringstream iss(line);
    string tmp ;
    while (iss>>tmp)
    {
        sv.push_back(tmp);
    }
}

int main()
{
    string line = "Your enterprise DBAs oversee one of your most valuable assets: the database. They need to plan, design, and deploy new architectures to meet the needs of your applications. They can’t do that if they are busy with routine tasks.";
    vector<string> sv;
    read(line, sv);
    int threshold = 6;
    auto c = count_if(sv.begin(), sv.end(), bind(short_word, _1, threshold));
    cout<<c<<endl;
    vector<int> iv = {1,2,3,4,5,6,7,8,9,10};
    const string t = "hello";
    auto i = find_if(iv.begin(), iv.end(), bind(small_number, _1, t));
    cout<<*i<<endl;
    return 0;
}