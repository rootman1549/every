#include <signal.h>
#include <stdio.h>


void func(){
    printf("hello\n");
    alarm(2);
}


int main(){
    signal(SIGALRM, func);
    alarm(2);
    while (1)
        ;
}

