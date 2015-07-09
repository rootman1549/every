#ifndef _AAA_H
#define _AAA_H

typedef struct AAA PAAA;

PAAA * creat_aaa();
int free_aaa(PAAA *aaa_obj);
int get_int(PAAA *aaa_obj);
void set_int(PAAA *aaa_obj, int num);

#endif
