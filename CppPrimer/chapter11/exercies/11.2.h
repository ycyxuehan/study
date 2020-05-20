#include "stdfx.h"

using namespace std;

void addFamily(map<string, vector<string>> families, string family, string member)
{
    if (family == "")
    {
        return;
    }
    vector<string> members = families[family];
    if (member != "")
    {
        members.push_back(member);
    }
}