#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
 * Student Name: Rehan Nagoor Mohideen
 * Student ID: 110592
 */

typedef struct {
    char ID;
    int size;//how much space the process needs
    int swaps;//the number of times it was swapped
    int strtTime;//used to order the queue
}Process;

/**
 * @return the index of the parent of the current burst
 */
int parent(int i) {
    return (i - 1) / 2;
}

/**
 * @return the index of the left child of the current burst
 */
int leftChild(int i) {
    return ((2 * i) + 1);
}

/**
 * @return the index of the right child of the current burst
 */
int rightChild(int i) {
    return ((2 * i) + 2);
}

/**
 * swap function for minheap structure
 */
void swap(Process **processList, int i, int j) {
    Process *temp = processList[i];
    processList[i] = processList[j];
    processList[j] = temp;
}
 
/**
 * shiftUp function for minheap structure
 */
void shiftUp(Process **processList, int i) {
    while (i > 0 && processList[parent(i)]->strtTime > processList[i]->strtTime) {
        swap(processList, parent(i), i);
        i = parent(i);
    }
}

/**
 * shiftDown function for minheap structure
 */
void shiftDown(Process **processList, int size, int i) {
    int maxIndex = i;
    int l = leftChild(i);

    if (l <= size && processList[l]->strtTime < processList[maxIndex]->strtTime) {
        maxIndex = l;
    }

    int r = rightChild(i);
    if (r <= size && processList[r]->strtTime < processList[maxIndex]->strtTime) {
        maxIndex = r;
    }

    if (i != maxIndex) {
        swap(processList, i, maxIndex);
        shiftDown(processList, size, maxIndex);
    }
}

/**
 * insert function for minheap structure.
 * adds the strtTime supplied to the new position and shifts to appropriate position
 */
void insert(Process **processList, int *size, int p) {
    *size = *size + 1;
    processList[*size]->strtTime = p;
    shiftUp(processList, *size);
}

/**
 * extractMin function for minheap structure
 * @returns the strtTime of the burst and removes the burst from the heap
 */
int extractMin(Process **processList, int *size) {
    int result = processList[0]->strtTime;
    processList[0] = processList[*size];
    *size = *size - 1;
    shiftDown(processList, *size, 0);
    return result;
}

/**
 * change priority function for minheap structure
 */
void changePriority(Process **processList, int size, int i, int p) {
    int oldp = processList[i]->strtTime;
    processList[i]->strtTime = p;
     
    if (p > oldp) {
        shiftUp(processList, i);
    } else {
        shiftDown(processList, size, i);
    }
}
 
/**
 * getMin function for minheap structure
 */
void *getMin (Process **processList) {
    return processList[0];
}

/**
 * Remove function for minheap structure
 */
void Remove(Process **processList, int *size, int i) {
    processList[i] = getMin(processList);

    shiftUp(processList, i);

    extractMin(processList, size);
}

/**
 * Function find the first available hole that is of atleast size long
 * @return the index of where the hole starts
 */
int findFirstFreeSpc(Process *memory[128], int size) {
    int spotIndex;
    int spotSize;
    for (int i = 0; i < 128; i++) {
        if (memory[i] == NULL) {//if its empty
            spotIndex = i;//keep track of the index
            spotSize = 0;
            for (; i < 128 && memory[i]==NULL; i++) {//check size of the hole
                spotSize++;
            }
            if (spotSize >= size) {//if its big enough to store the process return its index
                return spotIndex;
            }
        }
    }
    return -1;
}

/**
 * Function find the next available hole that is of atleast size long
 * @return the index of where the hole starts
 */
int findNextFreeSpc(Process *memory[128], int size, int index) {
    int spotIndex;
    int spotSize;
    if (index == -1) {
        index = 0;
    }
    int i = index;

    if (memory[i] == NULL) {//if its empty
        spotIndex = i;//keep track of the index
        spotSize = 0;
        for (; i < 128 && memory[i]==NULL; i++) {//check size of the hole
            spotSize++;
        }
        if (spotSize >= size) {//if its big enough to store the process return its index
            return spotIndex;
        }
    }

    if (i >= 127) {
        i = 0;
    } else {
        i++;
    }

    for (; i != index;) {//check the rest of the file till it loops around
        if (memory[i] == NULL) {//if its empty
            spotIndex = i;//keep track of the index
            spotSize = 0;
            for (; i < 128 && memory[i]==NULL && i != index; i++) {//check size of the hole
                spotSize++;
            }
            if (spotSize >= size) {//if its big enough to store the process return its index
                return spotIndex;
            }
        }

        if (i >= 127 && i != index) {
            i = 0;
        } else if (i != index) {
            i++;
        }
        
    }

    return -1;
}

/**
 * Function find the best fit hole
 * @return the index of where the hole starts
 */
int findBestFreeSpc(Process *memory[128], int size) {
    int spotIndex = -1;
    int spotSize = 200;
    int tempIndex;
    int tempSize;

    for (int i = 0; i < 128; i++) {
        if (memory[i] == NULL) {//if its empty
            tempIndex = i;//keep track of the index
            tempSize = 0;
            for (; i < 128 && memory[i]==NULL; i++) {//check size of the hole
                tempSize++;
            }
            if (tempSize >= size && tempSize < spotSize) {//if its a big enough hole and smaller than the previous hole
                spotSize = tempSize;
                spotIndex = tempIndex;
            }
        }
    }
    return spotIndex;
}

/**
 * Function find the worst fit hole
 * @return the index of where the hole starts
 */
int findWorstFreeSpc(Process *memory[128], int size) {
    int spotIndex = -1;
    int spotSize = 0;
    int tempIndex;
    int tempSize;

    for (int i = 0; i < 128; i++) {
        if (memory[i] == NULL) {//if its empty
            tempIndex = i;//keep track of the index
            tempSize = 0;
            for (; i < 128 && memory[i]==NULL; i++) {//check size of the hole
                tempSize++;
            }
            if (tempSize >= size && tempSize > spotSize) {//if its a big enough hole and smaller than the previous hole
                spotSize = tempSize;
                spotIndex = tempIndex;
            }
        }
    }
    return spotIndex;
}

/**
 * finds the oldest process in memory and removes it from memory.
 * @return the pointer to the process that is in memory the longest
 */
Process *swapProcess(Process *memory[128]) {
    Process *oldest = NULL;
    int oldestTime = 999;
    for (int i = 0; i < 128; i++) {
        if (memory[i] != NULL && memory[i]->strtTime < oldestTime) {
            oldest = memory[i];
            oldestTime = oldest->strtTime;
        }    
    }

    if (oldest == NULL) {
        return NULL;
    }
    
    //clear the space of the oldest process
    for (int i = 0; i < 128; i++) {
        if (memory[i] != NULL && memory[i]->ID == oldest->ID) {
            memory[i] = NULL;
        }    
    }

    return oldest;
}

/**
 * Inserts the process to the memory by adding to the pointers to the hole.
 * Assumes that the index is valid and the space it takes does not exceed memory bounds.
 */
void insertProcessToMEM(Process *memory[128], Process *nextP, int index) {
    for (int i = 0; i < nextP->size; i++) {
        memory[index+i] = nextP;
    }
}

/**
 * Frees any Processes left in memory
 */
void freeMem(Process *memory[128]) {
    for (int i = 0; i < 128; i++) {
        if (memory[i] != NULL) {
            Process *temp = memory[i];
            for (int j = 0; j < 128 ; j++) {//remove all references
                if (memory[j] != NULL && memory[j]->ID == temp->ID) {
                    memory[j] = NULL;
                }
            }
            free(temp);
        }
    }
}

/**
 * @return the no. of holes in memory
 */
int getNoHoles(Process *memory[128]) {
    int noHoles = 0;
    for (int i = 0; i < 128; i++) {
        if (memory[i] == NULL) {
            noHoles++;
            while (i < 128 && memory[i] == NULL) {
                i++;
            }
        }
    }
    return noHoles;
}

/**
 * @return the memory usage of the current state of the memory
 */
double getMemusage(Process *memory[128]) {
    int mem = 0;
    for (int i = 0; i < 128; i++) {
        if (memory[i] != NULL) {
            mem++;
        }
    }
    return (double)mem*100/128;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: ./holes [file name] [allocation stategy: first, best, next, worst]\n");
        exit(0);
    }

    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        printf("Error: could not find or open file \"%s\"\n" ,argv[1]);
        exit(0);
    }
    
    int fFlag = 0;
    int bFlag = 0;
    int nFlag = 0;
    int wFlag = 0;

    if (strcmp(argv[2], "first") == 0) {
        fFlag = 1;
    } else if (strcmp(argv[2], "best") == 0) {
        bFlag = 1;
    } else if (strcmp(argv[2], "next") == 0) {
        nFlag = 1;
    } else if (strcmp(argv[2], "worst") == 0) {
        wFlag = 1;
    } else {
        printf("Error: invalid allocation stategy of \"%s\" should be either first, best, next or worst\n" ,argv[2]);
        exit(0);
    }

    Process *processList[26];
    char line[100];
    int size = -1;
    int order = 0;
    while (fgets(line, 100, fp) != NULL) {
        Process *temp = malloc(sizeof(Process));
        sscanf(line, "%c %d", &(temp->ID), &(temp->size));
        temp->swaps = 0;
        //insert to queue
        processList[size+1] = temp;
        insert(processList, &size, order++);
    }
    fclose(fp);

    Process *memory[128];
    int i;
    for (i = 0; i < 128; i++) {//initialize all memory to NULL/empty
        memory[i] = NULL;
    }
    
    int noProcesses = 0, totalLoads = 0, avgProcesses = 0, avgHoles = 0;
    double cumMem = 0;
    int freeSpot = 0;
    while (size != -1) {
        Process *nextP = (Process *)getMin(processList);//get next process to be inserted
        extractMin(processList, &size);

        if (nFlag == 1) {
            freeSpot = findNextFreeSpc(memory, nextP->size, freeSpot);
        } else if (bFlag == 1) {
            freeSpot = findBestFreeSpc(memory, nextP->size);
        } else if (wFlag == 1) {
            freeSpot = findWorstFreeSpc(memory, nextP->size);
        } else if (fFlag == 1) {
            freeSpot = findFirstFreeSpc(memory, nextP->size);//get the spot to insert
        }
        
        while (freeSpot == -1) {//while there is no freeSpot make space and get the spt
            Process *temp = swapProcess(memory);
            if (temp == NULL) {
                printf("Error: process requires more space than in memory");
                exit(0);
            }
            
            temp->swaps++;
            noProcesses--;
            if (temp->swaps == 3) {//process is complete
                free(temp);
            } else {//reinsert into the queue
                processList[size+1] = temp;
                insert(processList, &size, order++);
            }
            
            if (nFlag == 1) {
                freeSpot = findNextFreeSpc(memory, nextP->size, freeSpot);
            } else if (bFlag == 1) {
                freeSpot = findBestFreeSpc(memory, nextP->size);
            } else if (wFlag == 1) {
                freeSpot = findWorstFreeSpc(memory, nextP->size);
            } else if (fFlag == 1) {
                freeSpot = findFirstFreeSpc(memory, nextP->size);//get the spot to insert
            }
        }

        //insert the process into the spot
        insertProcessToMEM(memory, nextP, freeSpot);
        noProcesses++;
        totalLoads++;
        avgProcesses += noProcesses;
        avgHoles += getNoHoles(memory);
        cumMem += getMemusage(memory);
        printf("%c loaded, #processes = %d, #holes = %d, %%memusage = %.0lf, cumulative %%mem = %.0lf\n", nextP->ID, noProcesses, getNoHoles(memory), getMemusage(memory), cumMem/totalLoads);
    }
    
    freeMem(memory);
    printf("Total Loads = %d, average # processes = %.2lf, average # holes = %.1lf, cumulative %% mem = %.0lf\n", totalLoads, (double)avgProcesses/(double)totalLoads, (double)avgHoles/(double)totalLoads, cumMem/totalLoads);

    return 0;
}
