#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Ass{
    int a;
    char c[0];
};

int main(){
    char *str = "abcdjjjjj";
    struct Ass *p = (struct Ass *)malloc(sizeof(struct Ass) + strlen(str) + 1);
    memset(p, '\0', sizeof(struct Ass) + strlen(str) + 1);
    memcpy(p->c, str, strlen(str));
    printf("%d\n", sizeof(struct Ass));
    printf("%s\n", p->c);
    free(p);
    p = NULL;
}
