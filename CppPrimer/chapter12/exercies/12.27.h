#include "stdfx.h"

using namespace std;
class QueryResult
{
    private:
        size_t count;
        set<int> found_line_indices;
        shared_ptr<vector<string>> ptr_text;
        string word;
    public:
        QueryResult(){}
        QueryResult(string w, set<int> s, shared_ptr<vector<string>> pvs):word(w), count(s.size()), found_line_indices(s), ptr_text(pvs){}
        size_t get_count(){return count;};
        set<int> * get_found_line_indices(){return &found_line_indices;};
        vector<string> get_lines();
        void display();
};

class TextQuery
{
    private:
        vector<string> text;
        map<string, set<int>> word_map;
    public:
        TextQuery(){}
        TextQuery(string f){read_file(f);};
        void read_file(string);
        QueryResult query(string);

};

