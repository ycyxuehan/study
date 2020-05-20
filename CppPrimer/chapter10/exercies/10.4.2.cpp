#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>


using namespace std;

void read(string file, vector<string> &sv)
{
    ifstream ifs(file);
    istream_iterator<string> isiter(ifs), eof;
    copy(isiter, eof, sv);
    ifs.close();
}

void read(vector<int> &iv)
{
    istream_iterator<int> isiter(cin), eof;
    copy(isiter, eof, iv);
}

void unique_read(vector<int> &iv)
{
    istream_iterator<int> isiter(cin), eof;
    unique_copy(isiter, eof, iv);
}

void readWrite1033(string in, string odd, string even)
{
    ifstream ifs(in);
    ofstream ofsodd(odd);
    ofstream ofseven(even);

    istream_iterator<int> isiter(ifs), eof;
    ostream_iterator<int> ositer_odd(ofsodd, "\n");
    ostream_iterator<int> ositer_even(ofseven, "\n");
    while (isiter != eof)
    {
        int i = *isiter++;
        if(i%2)
        {
            //odd
            ositer_odd = i;
        }
        else
        {
            //even
            ositer_even = i;
        }
        
    }
    
}