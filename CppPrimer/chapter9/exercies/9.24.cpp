#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::vector;

void display_front(const vector<int> &vi)
{
    cout << "items of vector: ";
    for(auto cbeg = vi.cbegin(); cbeg != vi.cend(); ++cbeg)
    {
        cout << *cbeg <<",";
    }
    cout<<endl;
    cout <<"front itme of vector: at: "<<vi.at(0) << ", [0]: "
        <<vi[0]<< ", front(): " <<vi.front()
        << ", begin(): "<<*vi.begin()<<endl;
}

int main()
{
    vector<int> vi = {1, 2, 3};
    vector<int> empty;
    display_front(vi);
    //throw an expection
    // display_front(empty);
    return 0;
}