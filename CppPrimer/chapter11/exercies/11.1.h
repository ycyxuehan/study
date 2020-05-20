#include "stdfx.h"

using namespace std;

void count_word(istream &is, map<string, size_t> &word_count)
{
    string word;
    while (is>>word)
    {
        ++word_count[word];
    } 
}

void count_wordEx(istream &is, map<string, size_t> &word_count)
{
    string word;
    set<char> ignore = {',', '.', '?', '!'};
    while (is>>word)
    {
        //去除标点符号
        if(ignore.find(*word.rbegin()) != ignore.end())
        {
            word.erase(word.rbegin().base());
        }
        transform(word.begin(),word.end(), word.begin(), ::tolower);
        ++word_count[word];
    } 
}

