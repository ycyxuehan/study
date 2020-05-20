#include "6.7.h"

int isUsed(void)
{
    static int used = 0;
    return used++;
}

