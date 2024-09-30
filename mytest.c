#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <string.h>
#include <sys/time.h>

// Compile with -DREALMALLOC to use the real malloc() instead of mymalloc()
#ifndef REALMALLOC
#include "mymalloc.h"
#endif

void t0() { //malloccing 0 bytes

    int* a = malloc(0);
    free(a);

    printf("%i\n", checkLeaky());

}
void t1() { //malloccing the entire memory

    int* fullMem = malloc(4088);
    free(fullMem);

    printf("%i\n", checkLeaky());
}

void t2() { //freeing something not obtained from malloc

    int x = 5;
    free(&x);

}

void t3() { //freeing in the middle of memory

    int* x = malloc(10);
    int* y = x + 1;

    free(y);
    free(x);

    printf("%i\n", checkLeaky());

}

void t4() { //freeing something twice

    int* x = malloc(10);
    
    free(x);
    free(x);

}

void t5() { //testing coalescence

    int* x = malloc(8);
    int* y = malloc(8);

    free(x);
    free(y);

    int* z = malloc(16);

    free(z);

    printf("%i\n", checkLeaky());
}

void t6() { //ensuring there is no object overlap

    int* x = malloc(4);
    int* y = malloc(4);

    *x = 1;
    *y = 2;

    printf("%i\n", *x);
    printf("%i\n", *y);

    free(x);
    free(y);

    printf("%i\n", checkLeaky());
}

void t7() { //mallocing too many bytes

    int* x = malloc(5000);
    free(x);
}

int main() {

    //uncomment the test to be checked

    //t0();
    //t1();
    //t2();
    //t3();
    //t4();
    //t5();
    //t6();
    //t7();
}