#include <fstream>
#include <string>
#include <vector>
#include <iostream>
using std::cout;
using std::endl;
using std::ifstream;
using std::string;
using std::vector;
using std::getline;
using std::ofstream;

bool readWord(string filename, vector<string> &vs)
{
    ifstream ifs(filename);
    if (ifs)
    {
        string line;
        while (ifs >> line)
        {
            /* code */
            vs.push_back(line);
        }
        return true;
    }
    return false;
}

bool readLine(string filename, vector<string> &vs)
{
    ifstream ifs(filename);
    if (ifs)
    {
        string line;
        while (getline(ifs, line))
        {
            /* code */
            vs.push_back(line);
        }
        return true;
    }
    return false;
}

bool write(string filename, const vector<string> &vs)
{
    ofstream ofs(filename, ofstream::app);
    if(ofs)
    {
        for(auto cbeg = vs.cbegin(); cbeg != vs.cend(); ++cbeg)
        {
            ofs<<*cbeg<<endl;
        }
        return true;
    }
    return false; 
}

int main()
{
    string filename = "/root/git/cppstudy/chapter8/exercies/8.1.cpp";
    vector<string> vs;
    auto r = readLine(filename, vs);
    if (r)
    {
        for (auto line:vs)
        {
            cout << line<<endl;
        }
    }
    else
    {
        cout << "read error." << endl;
    }
    r = write("/tmp/tmp.cpp", vs);
    if(r)
    {
        cout<<"written！";
    }
    else
    {
        cout<<"write error!";
    }
    
    return 0;
}