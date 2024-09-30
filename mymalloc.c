#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mymalloc.h"

#define MEMLENGTH 512
static double memory[MEMLENGTH];

#define ROUND(x) ((x+7) & (-8))

int getSize(char* p);
int getAllocated(char* p);
void setSize (char *p, int size);
void setAllocated (char *p, int alloc);

//if getAllocated = 0 -> chunk is unallocated
//if getAllocated = 1 -> chunk is allocated
//if getAllocated = 2 -> chunk has been freed

void *mymalloc(size_t size, char *file, int line) {

    //hard test for size == 0 

    if(size <= 0 ) {
        fprintf(stderr, "Cannot malloc 0 bytes! %s:%i\n", file, line);
        exit(EXIT_FAILURE);
    }

    //round 

    size = ROUND(size);

    char *currentPosition = ((char*) memory) + 8; //pointer set up right before initial payload
    char *end = ((char*) (memory + MEMLENGTH)); //pointer to end of memory array

    while(currentPosition < end) {

        if(getSize(currentPosition) == 0 && (double *)(currentPosition-8) == memory) { //check for if at start of array

            if(size > MEMLENGTH*8 - 8) { //if request is too great
                fprintf(stderr, "Cannot malloc that many bytes! %s:%i\n", file, line);
                exit(EXIT_FAILURE);
            }

            setSize(currentPosition, size); //update size of the chunk
            
            setAllocated(currentPosition, 1); //set it to allocated

            void* copy = currentPosition; //copy over current position

            //going to next chunk and update metadata accordingly
            if(MEMLENGTH * 8 - 8 > size) {

                currentPosition = currentPosition + size + 8;
                setSize(currentPosition, (MEMLENGTH*8) - size - 16);
                setAllocated(currentPosition, 0);

            }
            

            return copy; //returned the copied current position
        }
        
        //if size of the chunk is greater than requested size and is unallocated/has been freed
        if((getSize(currentPosition) >= size) && (getAllocated(currentPosition) == 0 || getAllocated(currentPosition) == 2)) {
            
            //update chunk metadata
            int originalSize = getSize(currentPosition); 
            setSize(currentPosition, size);
            setAllocated(currentPosition, 1);

            void* copy = currentPosition; //copy over to the current position

            //updat the next chunk as long as there is room left
            if(originalSize > size) {

                currentPosition = currentPosition + size + 8;
                setSize(currentPosition, originalSize - size - 8);
                setAllocated(currentPosition, 0);
    
            }

            return copy;
        }

        //update the current position
        currentPosition = currentPosition + getSize(currentPosition) + 8;
    }

    //not enough free space
    return NULL;
}

void myfree(void *ptr, char *file, int line) {
    
    char *currentPosition = ((char*) memory) + 8; //maintain the current position
    char *end = ((char*) (memory + MEMLENGTH)); //maintain the end of the memory array
    char *prevPointer = NULL; //maintain the location of the previous chunk

    //checks if requested error is out of bounds of memory array
    if((double*)ptr < memory || (char*)ptr > end) {

        fprintf(stderr,"Pointer is not contained within the heap. %s:%i\n", file, line); //addresses error 1
        exit(EXIT_FAILURE);
    }

    //makes sure the code doesn't reach the end
    while(currentPosition < end) {

        //checks if the current chunk is the one we are trying to free
        if(currentPosition == ptr) {

            //checks if the chunk has been allocated
            if(getAllocated(currentPosition) == 0) {
                fprintf(stderr,"Pointer has not been allocated. %s:%i\n", file, line);
                exit(EXIT_FAILURE);
            }
            
            //checks if chunk has already been freed
            if(getAllocated(currentPosition) == 2) { //meaning that the pointer has already been freed
                fprintf(stderr,"Pointer has already been freed. %s:%i\n", file, line);  //addresses error 3
                exit(EXIT_FAILURE);
            }

            //updates allocated metadata to freed
            setAllocated(currentPosition, 2);

            //check if previous chunk is free, and update metadata
            if(prevPointer!= NULL) {

                if(getAllocated(prevPointer) == 0 || getAllocated(prevPointer) == 2) {

                setSize(prevPointer, getSize(prevPointer) + 8 + getSize(currentPosition));
                currentPosition = prevPointer;

                }
            }

            //check if next chunk is free, and update metadata
            if((currentPosition + getSize(currentPosition) + 8) < end) {


                if(getAllocated(currentPosition + getSize(currentPosition) + 8) == 0 || getAllocated(currentPosition + getSize(currentPosition) + 8) == 2) {
                
                setSize(currentPosition, getSize(currentPosition) + 8 + getSize(currentPosition + getSize(currentPosition) + 8));

                }
                
            }

            return;
        }

        //update prev pointer and current position
        prevPointer = currentPosition;
        currentPosition = currentPosition + getSize(currentPosition) + 8;
    }

    //if it reaches this pointer, pointer is within bounds, but is not at a chunk
    fprintf(stderr,"Pointer address is not at the start of the chunk. %s:%i\n", file, line); //addresses error 2
    exit(EXIT_FAILURE);
}

int checkLeaky() { //returns 1 if memory isn't leaky, return 0 if memory is leaky

    char* start = ((char*)memory) + 8;

    if((getSize(start)==0 && getAllocated(start) == 0) || ((getSize(start) == 8 * MEMLENGTH - 8) && (getAllocated(start) == 0 || getAllocated(start) == 2))) {
        return 1; //all good
    }

    return 0; //problem case
}

int getSize(char* p) {

    //returns value 8 bytes back
    return *((int*) (p-8));
}

int getAllocated(char* p) {

    // returns 0 if unallocated, returns 1 if allocated, returns 2 if already freed
    return *((int*) (p-4));
}

void setSize (char *p, int size) {

    *((int*) (p-8)) = size;
}

void setAllocated (char *p, int alloc) {

    *((int*) (p-4)) = alloc;
}