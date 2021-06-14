#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_DATA_LENGTH 30
#define MAX_BUFF_LEN 5

typedef struct _s_memoryManager {
 __uint16_t blockCount;
 __uint16_t actualMemoryUsed;  //actual memory of all valid blocks;
 __uint16_t currentFreeIndex; //next index where a record can be added
 __uint16_t defragmented; //whether already defragmented, reset this if delete performed
} _s_memoryManager;

int main(){
    FILE* fp;
    _s_memoryManager memManager;

    // __uint16_t memManager[4];
    __uint8_t data[MAX_DATA_LENGTH];
    __uint16_t ch;
    __uint8_t buffer[MAX_BUFF_LEN];
    __uint16_t lineCounter = 0;
    __uint16_t readCount = 0;

    int buffCount=0;
    int arrCount = 0;

    fp = fopen("data.txt", "r");
    if(fp == NULL){
        printf("Error! Could not open memory file.\n");
        exit(1);
    }

    while(1){
        ch = fgetc(fp);

        if(ch == ' '|| ch == '\n' || feof(fp)){ 
            buffCount = 0;
            readCount++;
            printf("Read count: %hu\n", readCount);
            if(lineCounter == 0){
                switch(readCount){
                    case 1: memManager.blockCount = atoi(buffer);
                            break;
                    case 2: memManager.actualMemoryUsed = atoi(buffer);
                            break;
                    case 3: memManager.currentFreeIndex = atoi(buffer);
                            break;
                    case 4: memManager.defragmented = atoi(buffer);
                            break;
                    default: printf("Error in line 52.\n\n");
                            exit(1);
                }
                if(ch == '\n') lineCounter++;
            }
            
            else data[arrCount++] = atoi(buffer);
            bzero(buffer,3);
            if(feof(fp)){
                printf("Reached EOF. Closing!\n");
                break;
            }
            continue;
        }

        if(feof(fp)){
            printf("Reached EOF. Closing!\n");
            break;
        }
        buffer[buffCount++] = ch;
        // buffCount++;
        // if(ch == 65487) break;
        printf("%hu\n", ch);
    }
    fclose(fp);

    printf("\n\n");
    printf("%hu\n", memManager.blockCount);
    printf("%hu\n", memManager.actualMemoryUsed);
    printf("%hu\n", memManager.currentFreeIndex);
    printf("%hu\n", memManager.defragmented);
    for(int j = 0; j<MAX_DATA_LENGTH; j++){
        printf("%hu\n", data[j]);
    }
}