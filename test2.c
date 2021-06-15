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
        if(ch == ' ') continue;
        if(ch == '\n'){
            lineCounter++;
            continue;
        }

        if(feof(fp)){
            printf("Got End of file. Now closing.\n");
                fclose(fp);

            printf("\n\n");
            printf("%hu\n", memManager.blockCount);
            printf("%hu\n", memManager.actualMemoryUsed);
            printf("%hu\n", memManager.currentFreeIndex);
            printf("%hu\n", memManager.defragmented);
            for(int j = 0; j<MAX_DATA_LENGTH; j++){
                printf("%hu\n", data[j]);
            }
            exit(0);
        }
        
        //Now, in case the above three cases don't occur, we have valid data which has to be saved
        //such data is of two types, __uint16_t types on first line to be saved to memManager
        //__uint8_t types data in next line to be saved in data array
        if(lineCounter==0){
            ch = ch -'0'; //converting from character to number
            readCount++;
            switch(readCount){
                case 1: memManager.blockCount = ch;
                        break;
                case 2: memManager.actualMemoryUsed = ch;
                        break;
                case 3: memManager.currentFreeIndex = ch;
                        break;
                case 4: memManager.defragmented = ch;
                        break;
                default: printf("Error in line 52.\n\n");
                        exit(1);
            }
        }

        else data[arrCount++] = ch-'0';
        printf("%hu\n", ch);
    }
}