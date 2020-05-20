#include "stdfx.h"

template<class T, size_t Ind> constexpr size_t size(T (&t)[Ind])
{
    return Ind;
}