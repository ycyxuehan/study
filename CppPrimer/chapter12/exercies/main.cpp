#include "stdfx.h"
#include "12.27.h"

int main()
{
    TextQuery tq(string("test.txt"));
    auto qr = tq.query(string("and"));
    qr.display();
    return 0;
}