#include "12.27.h"

using namespace std;

void TextQuery::read_file(string f)
{
    ifstream ifs(f);
    string line;
    int line_no = 1;
    text.push_back("not 0 line!");
    while (getline(ifs, line))
    {
        text.push_back(line);
        istringstream iss(line);
        string word;
        while (iss>>word)
        {
            word_map[word].insert(line_no);
        }
        ++ line_no;
    }
}

QueryResult TextQuery::query(string word)
{
    auto iter = word_map.find(word);
    if(iter != word_map.end())
    {
        auto sptr = make_shared<vector<string>>(text);
        QueryResult qr(word, iter->second, sptr);
        return qr;
    }
    return QueryResult();
}

vector<string> QueryResult::get_lines()
{
    vector<string> sv;
    for(auto i : found_line_indices)
    {
        sv.push_back((*ptr_text)[i]);
    }
    return sv;
}

void QueryResult::display()
{
    cout<<count<<" '" << word<<"' found:" <<endl;
    for(auto i : found_line_indices)
    {
        cout<<i<<": "<<(*ptr_text)[i]<<endl;
    }
}
