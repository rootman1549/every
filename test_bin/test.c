#include <stdio.h>
#include <sys/stat.h>
#include "ccc.h"

int main()
{
    /*
    1. write to file
    2. run process
    */

    FILE *fp = NULL;
    fp = fopen("bbb", "wb");
    fwrite(data, sizeof(char), sizeof(data), fp);
    fclose(fp);
    chmod("bbb", S_IRUSR | S_IWUSR | S_IXUSR);
    system("./bbb");
}



