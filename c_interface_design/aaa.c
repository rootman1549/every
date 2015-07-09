#include "stdio.h"
#include "stdlib.h"
#include "aaa.h"

struct AAA
{
    int aaa;
};

PAAA * creat_aaa()
{
    PAAA *pa = (PAAA *)malloc(sizeof(struct AAA));
    if (pa == NULL){
        return NULL;
    }
    
    pa->aaa = 8;
    printf("----func_aaa----: %d\n", pa->aaa);
    
    return pa;
}


int free_aaa(PAAA *aaa_obj)
{
    if (aaa_obj == NULL){
        return -1;
    }
    
    free(aaa_obj);
    aaa_obj = NULL;
 
    printf("----free_aaa----\n");
    return 0;
}


int get_int(PAAA *aaa_obj)
{
    return aaa_obj->aaa;
}

void set_int(PAAA *aaa_obj, int num)
{
    aaa_obj->aaa = num;
}