#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

bool longWord(const string &word)
{
    return word.size() >= 5;
}

void read(const string &s, vector<string> &sv)
{
    istringstream iss(s);
    string tmp = "";
    while (iss>>tmp)
    {
        sv.push_back(tmp);
    }    
}

int main()
{
    vector<string> sv;
    string line = "Your enterprise DBAs oversee one of your most valuable assets: the database.";
    read(line, sv);
    auto long_word_end = partition(sv.begin(), sv.end(), longWord);
    auto beg = sv.begin();
    cout<<line<<endl;
    while (beg != long_word_end)
    {
        cout << *beg << " ";
        ++beg;
    }
    cout<<endl;
    
    return 0;
}