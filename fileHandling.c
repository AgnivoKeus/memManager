#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_ARR_LENGTH 30

__uint8_t arr[MAX_ARR_LENGTH];
__uint16_t remLen = MAX_ARR_LENGTH;

typedef struct _s_memoryManager {
 __uint16_t blockCount;
 __uint16_t actualMemoryUsed;  //actual memory of all valid blocks;
 __uint16_t currentFreeIndex; //next index where a record can be added
 __uint16_t defragmented; //whether already defragmented, reset this if delete performed
} _s_memoryManager;

_s_memoryManager memManager;

void clearInput(){//it will clear of any extra characters from the input buffer
	int c = getchar();
	while(c!='\n' && c!= EOF)
		c = getchar();
}
//create function for printing memManager


char inputId(){
	__uint8_t id;
    while(1){
        printf("Enter ID(Valid id: 0 - 254): ");
        scanf(" %hhu", &id);
        clearInput();
        printf("\n\n");
        if(id == 255) printf("255 is not a valid entry. ");
        else break;
    }
	return id;
}

void printArray(){
	for(int i = 0; i<MAX_ARR_LENGTH; i++)
		printf("%hhu ",arr[i]);
	printf("\n\n");
}


bool validateId(__uint8_t id){
	__uint16_t i = 0;
    if(memManager.blockCount == 0) return true;

	while(i<memManager.currentFreeIndex){
		if(arr[i] && arr[i+1] == id) //if entry is valid and id matches arr[i] && 
			return false;
		else i = i+ 3 + arr[i+2];
	}
	return true;
}

void invalidateId(__uint8_t id){
    __uint16_t i = 0;
    if(memManager.blockCount == 0){
        printf("Database is empty. \n\n");
        return;
    }

	while(i<memManager.currentFreeIndex){
		if(arr[i] && arr[i+1] == id){   //if entry is valid and id matches
				arr[i] = false;
                memManager.blockCount--;
                memManager.defragmented = false;
                memManager.actualMemoryUsed = memManager.actualMemoryUsed - (arr[i+2] + 3); // decreasing by xDataLen
                return;
        }
		else i = i+ 3 + arr[i+2];
	}
	printf("Could not remove data. Id does not exist\n\n");
}

void deleteRecord(__uint16_t pos){
    while(pos<memManager.currentFreeIndex){
        __uint8_t xDataLen = arr[pos+2];
        __uint16_t nextIdPos = pos + xDataLen + 3;
        __uint16_t nextEntryLen = arr[nextIdPos + 2];
        __uint16_t newCurrLen = memManager.currentFreeIndex - (xDataLen+3);
        __uint16_t destIndex = pos;
        __uint16_t sourceIndex = nextIdPos;

        while(destIndex < newCurrLen){
            arr[destIndex] = arr[sourceIndex];
            destIndex++;
            sourceIndex++;
        }

        __uint16_t j=destIndex;
        while(j<memManager.currentFreeIndex){ //
                arr[j] = 0;
            j++;
        }
        
        memManager.currentFreeIndex = newCurrLen;
        return;
    }
}

void defragment(){
    __uint16_t i =0;
    printArray();
    __uint16_t currentFreeIndex = memManager.currentFreeIndex;
    while(i<memManager.currentFreeIndex){
        if(!arr[i]) //if entry is invalid
			deleteRecord(i);
		else i = i+ 3 + arr[i+2];
    }

    __uint16_t j = memManager.currentFreeIndex;
    while(j<currentFreeIndex){ //
        arr[j] = 255;
        j++;
    }
    memManager.defragmented = true;
    printArray();
}


void addEntry(__uint8_t id, bool overWrite){
    //if called from overwrite, prev id has already been invalidated
    //take new length input, if(len>virtFreeSpace&&len<remSpace), then defrag, then add

    if(!overWrite&&validateId(id) && (remLen<3)){ //if only less than three bytes of actual space
                                                  //is remaining and we are not performing overWrite
        printf("Not enough memory for new entry. Aborting process. \n\n");
        return;
    }
    char ch;
    __uint8_t dataLen = 0;
    __uint8_t xDataLen = 0;
    __uint16_t i =0;
    __uint16_t remSpace =0; 
    remLen = MAX_ARR_LENGTH - memManager.actualMemoryUsed;

    //getting xdataLen
    if(overWrite){
        while(i<=memManager.currentFreeIndex){
            if(arr[i] && arr[i+1] == id){
                xDataLen = arr[i+2];
                break;
            }   
            else i = i = i+ 3 + arr[i+2];
        }
    } 
    // if(overWrite) remSpace = remLen; //Here, we have already invalidated the previous data
    // else  //valid, id, dataLen will be stored for new entry
    remSpace = remLen - 3;
    if(remSpace > MAX_ARR_LENGTH) remSpace = 0;

    __uint16_t virtRemSpace = MAX_ARR_LENGTH - memManager.currentFreeIndex - 1;
    if(virtRemSpace > MAX_ARR_LENGTH) virtRemSpace = 0;

    if(remSpace != 0){
        //here, remLen>3, if it is 3, then dataLen has to be forced to Zero
        while(1){ //keep iterating till a valid dataLen is given
            printf("Enter length of data: ");
            scanf(" %hhu", &dataLen);
            clearInput();
            if(dataLen > remSpace)
                printf("Not enough memory, you can only store %hu bytes of data\n\n", remSpace);
            else break;
        }
    }

    if(dataLen>=virtRemSpace && dataLen<=remSpace)
        defragment(); 
    //else dataLen = 0, already initialised as 0

    arr[memManager.currentFreeIndex] = true; //valid- true
    arr[memManager.currentFreeIndex + 1]= id; 
    arr[memManager.currentFreeIndex + 2] = dataLen;


    if(dataLen != 0) printf("Enter %hhu bytes of data:-\n", dataLen);
    else printf("Zero databytes can be stored for the given entry. Creating entry with zero data bytes\n\n");
    i = 1;
    bool readFlag;
    while(i<=dataLen){ //will not be executed if dataLen = 0
        while(1){
            readFlag = false;
            int retVal = scanf(" %hhu", &arr[memManager.currentFreeIndex+2+i]);
            if(retVal == 1){
                readFlag = true;
                break;
            }
            else if(retVal == 0){
                clearInput();
                printf("Invalid input given. Valid input: 0-254\n");
                readFlag = false;
                break;
            }
            printf("%hhu ", i);
        }
        if(readFlag == true) i++;
    }
    
    clearInput();
    
    memManager.currentFreeIndex = memManager.currentFreeIndex + dataLen+3;
    //if overWrite, xDataLen will not necessarily be 0
    memManager.actualMemoryUsed = memManager.actualMemoryUsed+dataLen-xDataLen+3;

    memManager.blockCount++;
    remLen = MAX_ARR_LENGTH - memManager.actualMemoryUsed;
}


void overWriteFunc(__uint8_t id){

    if(validateId(id) && (remLen<3)){
        printf("Not enough memory for new entry. Aborting process. \n");
        return;
    }

    char ch;
    remLen = MAX_ARR_LENGTH - memManager.actualMemoryUsed;
    __uint16_t xDataLen = 0;
	__uint16_t remSpace = 0;

    int i = 0;
    
    while(i<memManager.currentFreeIndex){
        if(arr[i] && arr[i+1]==id){
            xDataLen = arr[i+2];
            remSpace = remLen + xDataLen;
            break;
        }
        else i = i + 3 + arr[i+2];
    }
    
    printf("You will be able to store %hu bytes of total data for id: %hhu.\n", remSpace, id);
    while(1){
        printf("Are you sure you wish to overwrite the existing data?\n");

        printf("Enter y for yes and n for no: ");
        scanf(" %c", &ch);
        if(ch == 'y'){
            invalidateId(id);
            addEntry(id, true);
            break;
        }
        else if(ch == 'n') return;
        else printf("Please enter valid input\n");
    }
}

void storeNewData(){
    remLen = MAX_ARR_LENGTH - memManager.actualMemoryUsed;
    

    //Possible cases:-
    //Remaining memory is <=3, then abort
    //id does not match with any valid entry, go for addEntry
    //id matches with a valid entry, go for overWrite

    if(remLen==0){ //memory full
        printf("Memory full.\n");
        overWriteFunc(inputId());
        return;
    }
    __uint8_t id = inputId();
    
    if(memManager.actualMemoryUsed == 0){ //memory empty
        addEntry(id, false);
        return;
    }

    if(!validateId(id))
        overWriteFunc(id);
    else addEntry(id, false);
}

void printData(__uint8_t id){
    __uint16_t i = 0;
	while(i<memManager.currentFreeIndex){
		if(arr[i] && arr[i+1] == id){//if entry is valid and id matches
            __uint8_t dataLen = arr[i+2];
            for(int j = 0; j <=2+dataLen; j++)
                printf("%hhu ", arr[i+j]);
            printf("\n\n");
            return;
        } 
		else i = i+ 3 + arr[i+2];
	}
    printf("Could not print data. Id not in database.\n\n");
}

void printMemManager(){
    printf("Actual memory used: %hu\n", memManager.actualMemoryUsed);
    printf("Block count: %hu\n", memManager.blockCount);
    printf("CurrentFreeIndex: %hu\n", memManager.currentFreeIndex);
    printf("Defragmented: %hu\n",memManager.defragmented);
}

int main(){
    __uint8_t ch;
    
    FILE *fp;
    fp = fopen("data.txt", "r");
    if(fp == NULL){
        printf("Error! Could not open memory file.\n");
        exit(1);
    }
    
    int num;
    fscanf(fp, "%d", &num);
    printf("\n\n%d\n\n", num);

    memManager.blockCount = 0;
    memManager.actualMemoryUsed = 0;
    memManager.currentFreeIndex = 0;
    memManager.defragmented = true;
    int i = 0;
    
    while(i<MAX_ARR_LENGTH){
        arr[i] = 255;
        i++;
    }

	while(1){
		printf("Choose an operation from the following:\n");
		printf("1. Store new Data\n");
		printf("2. Delete existing data\n");
		printf("3. Print existing data\n");
		printf("4. Force defragment\n");
        printf("5. Print full array\n");
        printf("6. Print memory manager\n");
        printf("7. Exit\n\n");

		while(1){
			printf("Enter your choice:");
			int retVal = scanf(" %hhu",&ch);
			printf("\n");
			clearInput();

			if(retVal == 1 || retVal == EOF) break;
			else if(retVal == 0){
				printf("Bad Input\n");
				clearInput();
			}
		}

		switch(ch){
			case 1: storeNewData();
				break;
			case 2: invalidateId(inputId());
				break;
			case 3: printData(inputId());
				break;
			case 4: defragment();
            
			case 5: printArray();
				break;
            case 6: printMemManager();
                break;
            case 7: exit(0);
			default: printf("Invalid Choice\n\n");
		}
	}
}
