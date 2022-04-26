#include "stdio.h"

typedef enum {
    a = 0,
    b,
    c,
} a_t;


typedef struct {
    char a[2];
} b_t;

void test_ptr(void *k, void *j) 
{   
    void **b;
    b_t *a;
    b_t c = {{0,0}};
    a = &c;
    b = &c;
    
    printf("%d\n", a);
    a++;
    printf("%d\n", a);

    printf("%d\n", b);
    printf("%x\n", *b);
    printf("%x\n", *(char *)(6421920));
    printf("%x\n", *(char *)(6421921));
    // printf("%d\n", );
    b++;

    printf("%d\n", b);

    printf("%d\n", k);
    
    printf("%d\n", (int *)k+1);
}



int main(void)
{   
    b_t p = {{1,1}};
    b_t q;
    test_ptr(&p, &q);
    printf("%d\n", q);
    printf("%d %d\n", q.a[0], q.a[1]);

    unsigned char pk[3] = {0, 0, 0};
    // for (int i = 0; i < 10000; i++) {
    //     pk[1] = i;
    //     printf("%d. [%d] [%d] [%d] \n", i, pk[0], pk[1], pk[2]);
    // }
    printf("%d\n", sizeof(pk));

    return 0;
}