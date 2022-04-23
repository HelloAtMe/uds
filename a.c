#include "stdio.h"

typedef enum {
    a = 0,
    b,
    c,
} a_t;


int main(void)
{
    a_t k;
    k = 4;
    printf("%d\n", k);
    return 0;
}