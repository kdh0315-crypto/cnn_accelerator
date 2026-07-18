#include "activation_fn.h"

void relu(int32_t x, int32_t *y)
{
    if (x < 0) *y = 0;
    else *y = x;
}