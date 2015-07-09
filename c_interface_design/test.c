#include "stdio.h"
#include "stdlib.h"
#include "aaa.h"

int main(void)
{
    PAAA *aaa_obj = NULL;
    aaa_obj = creat_aaa(aaa_obj);
    if (aaa_obj == NULL){
        return -1;
    }
    
    //printf("aaa_obj: %d\n", aaa_obj->aaa);
    printf("aaaaaa:%d\n", get_int(aaa_obj));
    set_int(aaa_obj, 5);
    printf("bbbbb:%d\n", get_int(aaa_obj));
    
    free_aaa(aaa_obj);
    
    return 0;
}
