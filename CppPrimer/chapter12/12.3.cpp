#include <iostream>
#include <map>
#include <string>
#include <set>
#include <memory>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;
using line_no = vector<string>::size_type;

class QueryResult
{
    friend ostream &print(ostream &, const QueryResult &);

public:
    QueryResult(string s, shared_ptr<set<line_no>> p, shared_ptr<vector<string>> f) : sought(s), lines(p), file(f) {}

private:
    string sought;
    shared_ptr<set<line_no>> lines;
    shared_ptr<vector<string>> file;
};

class TextQuery
{
public:
    TextQuery(istream &);
    QueryResult query(const string &) const;

private:
    shared_ptr<vector<string>> file;
    map<string, shared_ptr<set<line_no>>> wm;
};

TextQuery::TextQuery(istream &ifs) : file(new vector<string>)
{
    string text;
    while (getline(ifs, text))
    {
        file->push_back(text);
        int n = file->size() - 1;
        istringstream line(text);
        string word;
        while (line >> word)
        {
            auto &lines = wm[word];
            if (!lines)
                lines.reset(new set<line_no>);
            lines->insert(n);
        }
    }
}

QueryResult TextQuery::query(const string &sought)const
{
    static shared_ptr<set<line_no>> no_data(new set<line_no>);
    auto loc = wm.find(sought);
    if(loc == wm.end())
        return QueryResult(sought, no_data, file);
    else
        return QueryResult(sought, loc->second, file);
    
}

string make_plural(size_t n, string a, string b)
{
    if(n <= 1)
        return a;
    return a+b;
}

ostream &print(ostream &os, const QueryResult &qr)
{
    os << qr.sought << " occurs "<<qr.lines->size() <<" "
    <<make_plural(qr.lines->size(), "time", "s") <<endl;
    for(auto num : *qr.lines)
    {
        os << "\t(line "<<num + 1 << ") " << *(qr.file->begin() + num) <<endl;
    }
    return os;
}