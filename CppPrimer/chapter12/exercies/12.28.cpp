#include "stdfx.h"

using namespace std;

void read_file(string f, vector<string> &lines, map<string, set<int>> &word_map)
{
    ifstream ifs(f);
    string line;
    int line_no = 1;
    while (getline(ifs, line))
    {
        lines.push_back(line);
        istringstream iss(line);
        string word;
        while (iss >> word)
        {
            word_map[word].insert(line_no);
        }
        ++line_no;
    }
}

void find_word(string f)
{
    map<string, set<int>> word_map;
    vector<string> lines = {"not 0 line"};
    read_file(f, lines, word_map);
    cout << "input a word to search(q/Q to quit): ";
    string word;
    while (cin >> word)
    {
        if (word == "q" || word == "Q")
        {
            break;
        }
        auto iter = word_map.find(word);
        if (iter != word_map.end())
        {
            cout << "found " << iter->second.size() << " " << word << endl;
            for (auto i : iter->second)
            {
                cout << "line " << i << " : " << lines[i] << endl;
            }
        }
    }
}

void find_word_do(string f)
{
    map<string, set<int>> word_map;
    vector<string> lines = {"not 0 line"};
    read_file(f, lines, word_map);

    string word;
    do
    {
        cout << "input a word to search(q/Q to quit): ";
        if (word == "q" || word == "Q")
        {
            break;
        }
        auto iter = word_map.find(word);
        if (iter != word_map.end())
        {
            cout << "found " << iter->second.size() << " " << word << endl;
            for (auto i : iter->second)
            {
                cout << "line " << i << " : " << lines[i] << endl;
            }
        }
    } while (cin >> word);
}

int main()
{
    find_word("test.txt");
    return 0;
}