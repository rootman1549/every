#include <stdio.h>

int main()
{
    FILE *fp = NULL;

    fp = fopen("/tmp/fopentmp", "wb");
    fclose(fp);
    
    printf("hello world.\n");
}
