#include <vector>
using std::vector;

bool is_item_exists(vector<int>::const_iterator cbeg, vector<int>::const_iterator cend, int item)
{
    while (cbeg != cend)
    {
        if(*cbeg == item)
        {
            return true;
        }
        ++cbeg;
    }
    return false;
}

vector<int>::iterator findItem(vector<int>::iterator beg, vector<int>::iterator end, int item)
{
    while (beg != end)
    {
        if(*beg == item)
        {
            return beg;
        }
        ++beg;
    }
    return end;
} 