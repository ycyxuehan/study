#include <iostream>
#include <list>
#include <string>
#include <algorithm>
#include <sstream>

using namespace std;

list<string>::iterator long_word(list<string> &sl, size_t threshold)
{
    return partition(sl.begin(), sl.end(), [threshold](const string &s){return s.size()>= threshold;});
}

list<string>::iterator stable_long_word(list<string> &sl, size_t threshold)
{
    return stable_partition(sl.begin(), sl.end(), [threshold](const string &s){return s.size()>= threshold;});
}

void read(const string &line, list<string> &sl)
{
    istringstream iss(line);
    string tmp ;
    while (iss>>tmp)
    {
        sl.push_back(tmp);
    }
}

int main()
{
    string line = "Your enterprise DBAs oversee one of your most valuable assets: the database. They need to plan, design, and deploy new architectures to meet the needs of your applications. They can’t do that if they are busy with routine tasks.";
    list<string> sl;
    read(line, sl);
    size_t threshold = 5;
    auto long_word_end = long_word(sl, threshold);
    cout<<line<<endl;
    cout <<"long than "<< threshold << " words: ";
    for_each(sl.begin(), long_word_end, [](const string &s){cout<<s<<" ";});
    cout<<endl;

    list<string> sl2;
    read(line, sl2);
    auto stable_long_word_end = stable_long_word(sl2, threshold);
    cout <<"stable long than "<< threshold << " words: ";
    for_each(sl2.begin(), stable_long_word_end, [](const string &s){cout<<s<<" ";});
    cout<<endl;

    return 0;
}