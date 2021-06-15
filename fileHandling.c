#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_ARR_LENGTH 30

__uint8_t arr[MAX_ARR_LENGTH];
__uint16_t remLen = MAX_ARR_LENGTH;

__uint8_t runId; //for checking whether the non-volatile memory has changed
__uint8_t loopCount= 0; //to sync with memory

typedef struct _s_memoryManager {
 __uint16_t blockCount;
 __uint16_t actualMemoryUsed;  //actual memory of all valid blocks;
 __uint16_t currentFreeIndex; //next index where a record can be added
 __uint8_t defragmented; //whether already defragmented, reset this if delete performed
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
        if(id == 255) printf("255 is not a valid entry.\n ");
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

void writeToMemory(){
        /*
    <Memory manager size 20 bytes

    write LB blockCount
    write HB blockCount
    write LB actualMemoryUsed
    write HB actualMemoryUsed
    write LB currentFree index
    write HB currentFree index
    write byte defragemented
    <dummy read 7-19>

    byte 20
    write array[i]

    */
    FILE* fp;

    fp = fopen("data.txt", "w+");
    if(fp == NULL){
        printf("Cannot open file for saving data. Please try again.\n\n");
        return;
    }
    __uint8_t temp;
    runId++;
    fputc(runId, fp);
    temp = memManager.blockCount>>8;
    fputc(temp, fp); //HB of blockCount
    fputc(memManager.blockCount, fp); //LB
    
    temp = memManager.actualMemoryUsed>>8;
    fputc(temp, fp); //HB of actual memory used
    fputc(memManager.actualMemoryUsed, fp); //LB

    temp = memManager.currentFreeIndex>>8;
    fputc(temp,fp); //HB
    fputc(memManager.currentFreeIndex,fp);  //currFreeIndex LB

    fputc(memManager.defragmented,fp); //only 1 byte long

    for(int i = 8; i<20;i++){
        fputc('D', fp); //putting dummy data into reserved space
    }
    for(int i = 0; i <MAX_ARR_LENGTH; i++){
        fputc(arr[i], fp);
    }
    fclose(fp);
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
                writeToMemory();
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
                arr[j] = 255;
            j++;
        }
        
        memManager.currentFreeIndex = newCurrLen;
        return;
    }
}

void defragment(){
    __uint16_t i =0;
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
    writeToMemory();
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
    writeToMemory();
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
    printf("\n\n");
}

void initialiseData(){
    FILE* fp;
    __uint8_t ch;
    __uint16_t readCount = 0;
    __uint8_t temp = 0;
    int arrCount = 0;

    fp = fopen("data.txt", "r");
    if(fp == NULL){
        printf("Error! Could not open memory file.\n");
        exit(1);
    }

    while(1){
        ch = fgetc(fp);
        if(feof(fp)){
            printf("Got End of file. Now closing.\n");
            fclose(fp);
            if(readCount == 0){ //if the program is being run for the first time,
                                //initialise all fields
                memManager.blockCount = 0;
                memManager.actualMemoryUsed = 0;
                memManager.currentFreeIndex = 0;
                memManager.defragmented = 1;

                for(int i = 0; i <MAX_ARR_LENGTH; i++)
                    arr[i] = 255;
            }
            return;
        }
        //There is some data in non-volatile memory
        if(++readCount == 1){
            if(++loopCount == 1){ 
                //if first loop of program, then just update the runId and continue to
                //updating volatile memory
                runId = ++ch;
                continue;
            }
            //else if not the first loop and run id is changed, then the nonvolatile memory
            //has changed from elsewhere, so update volatile memory with that
            //else no need to update
            else if (runId == ch)
                return;
            else runId = ch; //update runId of current program with that of memory
        }

        if(readCount>20) arr[arrCount++] = ch;  //first 20 bytes are reserved for memManager
                                                //rest are all 1 byte data
        else{
            switch(readCount){    
                case 2: temp = ch; //block count HB
                        break;
                case 3: memManager.blockCount = (temp<<8)|ch; //block count LB
                        break;
                case 4: temp = ch<<8; //actual memory used HB
                        break;
                case 5: memManager.actualMemoryUsed = ch|temp; //actual memory used LB
                        break;
                case 6: temp = ch<<8;   //currFreeIndex HB
                        break;
                case 7: memManager.currentFreeIndex = ch|temp; //HB
                        break;
                case 8: memManager.defragmented = ch; //defragmented is 1 byte long
                        break;
                default: break;
            }
        }
    }
}

int main(){
    __uint8_t ch;
    
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
        initialiseData();
		switch(ch){
			case 1: storeNewData();
				break;
			case 2: invalidateId(inputId());
				break;
			case 3: printData(inputId());
				break;
			case 4: defragment();
                break;
			case 5: printArray();
				break;
            case 6: printMemManager();
                break;
            case 7: writeToMemory();
                    exit(0);
			default: printf("Invalid Choice\n\n");
		}
	}
}
