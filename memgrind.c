#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <string.h>
#include <sys/time.h>

// Compile with -DREALMALLOC to use the real malloc() instead of mymalloc()
#ifndef REALMALLOC
#include "mymalloc.h"
#endif

void t1() {

    for(int i = 0; i < 120; i++) {

        char *pointer = malloc(1); //will create 1 byte malloc 120 times
        free(pointer);  //releases


    }

}

void t2() {

    char *ptr[120]; //intialize array

    for(int i = 0; i < 120; i++) {

        ptr[i] = malloc(1); //malloc each component of the array with 1 byte

    }

    for(int i = 0; i < 120; i++) {
        
        free(ptr[i]);
        //printf("worked\n");
    }
}

void t3() {

    char* ptr[120]; //initialize array
    int pos = 0;

    while (pos < 120) { //iterate until there are 120 mallocs

        if(pos==0 || rand() % 2 == 0) { //random condition, 0 is malloced because you cannot free 0 if unallocated

            ptr[pos] = malloc(1);
            pos++;
        }

        else {
            pos--;
            free(ptr[pos]); //free the pointer
            ptr[pos] = 0;
        }
    }
    
    pos = 0; //rest pos value
    while(pos < 120) {
        free(ptr[pos]); //free the array
        pos++;

    }
}

void t4() {

    char *ptr[120]; //intialize array

    for(int i = 0; i < 120; i++) {

        ptr[i] = malloc(1); //malloc each component of the array with 1 byte

    }

    for(int i = 0; i < 120; i++) {
        
        free(ptr[120-i-1]); //free backwords
        
    }

}

void t5() {

    char *ptr[120]; //intialize array

    //malloc every byte
    for(int i = 0; i < 120; i++) {

        ptr[i] = malloc(1);

    }

    //free every other byte
    for(int i = 0; i < 120; i += 2) {

        free(ptr[i]);

    }

    for(int i = 1; i < 120; i += 2) {

        free(ptr[i]);
    }
}

int main(int argc, char** argv) {

    struct timeval starttime;
    struct timeval endtime;

    //test1
    gettimeofday(&starttime, 0);

    for(int i = 0; i < 50; i++) {
        
        t1();

    }

    gettimeofday(&endtime, 0);

    printf("Average run time for t1: %ld seconds %ld microseconds\n", (endtime.tv_sec-starttime.tv_sec)/50, (endtime.tv_usec-starttime.tv_usec)/50);
    
   //test2
    gettimeofday(&starttime, 0);

    for(int i = 0; i < 50; i++) {

        t2();

    }

    gettimeofday(&endtime, 0);

    printf("Average run time for t2: %ld seconds %ld microseconds\n", (endtime.tv_sec-starttime.tv_sec)/50, (endtime.tv_usec-starttime.tv_usec)/50);

    //test3
    gettimeofday(&starttime, 0);

    for(int i = 0; i < 50; i++) {

        t3();

    }

    gettimeofday(&endtime, 0);

    printf("Average run time for t3: %ld seconds %ld microseconds\n", (endtime.tv_sec-starttime.tv_sec)/50, (endtime.tv_usec-starttime.tv_usec)/50);

    //test 4
    gettimeofday(&starttime, 0);

    for(int i = 0; i < 50; i++) {

        t4();

    }

    gettimeofday(&endtime, 0);

    printf("Average run time for t4: %ld seconds %ld microseconds\n", (endtime.tv_sec-starttime.tv_sec)/50, (endtime.tv_usec-starttime.tv_usec)/50);

    //test5
    gettimeofday(&starttime, 0);
    
    for(int i = 0; i < 50; i++) {

        t5();

    }

    gettimeofday(&endtime, 0);

    printf("Average run time for t5: %ld seconds %ld microseconds\n", (endtime.tv_sec-starttime.tv_sec)/50, (endtime.tv_usec-starttime.tv_usec)/50);

}