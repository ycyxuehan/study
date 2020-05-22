#include <iostream>
#include <random>
#include <vector>
#include <string>

using namespace std;

void generate(vector<int> &ia, size_t max)
{
    uniform_int_distribution<size_t> u(1, max);
    default_random_engine e;
    for (auto b = ia.begin(); b != ia.end(); ++b)
    {
        *b = u(e);
    }
}
void display(string prefix, const vector<int> &ia)
{
    cout << prefix << ": ";
    for (auto i : ia)
    {
        cout << i << " ";
    }
    cout << endl;
}

void merge(vector<int> &ia, int b, int m, int n)
{

    int ia_l[m - b + 1];
    int ia_r[n - m];
    for (auto i = 0; i <= m-b; ++i)
    {
        ia_l[i] = ia[i+b];
    }
    for (auto i = 0; i < n - m; ++i)
    {
        ia_r[i] = ia[m + i + 1];
    }
    auto i = 0, j = 0;
    for (auto k = b; k <= n; ++k)
    {
        if (i<=m-b && ia_l[i] < ia_r[j])
        {
            ia[k] = ia_l[i];
            ++i;

        }
        else if(j < n-m)
        {
            ia[k] = ia_r[j];
            ++j;
        }
    }
    string prefix = "merge " + to_string(b)+ ", " + to_string(m) + ", "+to_string(n);
    display(prefix, ia);
}


void merge_sort(vector<int> &ia, int b, int e)
{
    if (b < e)
    {
        int m = (b + e) / 2;
        merge_sort(ia, b, m);
        merge_sort(ia, m + 1, e);
        merge(ia, b, m, e);
    }
}

int main()
{
    vector<int> ia(10);
    generate(ia, 100);
    display("source", ia);
    merge_sort(ia, 0, 9);
    display("sorted", ia);
    return 0;
}