#include <stdio.h>

int main()
{
    int x = 65535;
    char y = 0b01;

    printf("%d\n", (y & (x << 1 | 1)));

    puts("Hello World!");



    return 0;
}