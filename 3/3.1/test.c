#include <stdio.h>

int f(int * a, int * b)
{
    printf("ok\n");

    *a = 2;
    *b = 3;
    
    return *a;
}

int main()
{
    int p, q, r;
    r = f(&p, &q);

    printf("r=%d,p=%d,q=%d\n", r,p,q);
    
    return 0;
}
