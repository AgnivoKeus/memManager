#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_COUNT 2
#define MAX_ARR_LENGTH 100

//int MAX_ARR_LENGTH = 20;
char arr[MAX_ARR_LENGTH];

int count = 0;
int currLen = 0;
//int remLen= MAX_ARR_LENGTH - currLen;

void clearInput(){//it will clear of any extra characters from the input buffer
	int c = getchar();
	while(c!='\n' && c!= EOF)
		c = getchar();
}

char inputId(){
	char id;
 	//clearInput();
	printf("Enter ID:");
	scanf(" %c", &id);
	printf("\n");
	clearInput();	
	return id;
}

bool validateID(char id){//checks if an id is already in use, returns false if already in use
	if(count == 0) return true;
	for(int i =0; i <currLen-2; i++){
		if(arr[i] == id)
			return false;
	}
	return true;
}

void leftShift(int pos, int newDataLen){
	//Here pos is the index of the id field of the entry being overWritten
	//pos+1 is index of dataLen
	//pos+1+XdataLen is the end of entry (xDatalen -> existing dataLen arr[pos+1])
	//we should shift from pos+1+xdataLen+1 position to pos+1+dataLen+1
	int i=0;
	int xDataLen = arr[pos+1] - '0'; //previous data length
					// when deleting an entry, arr[pos + 1] will always be a char
	int destIndex = pos + 2 + newDataLen +i;
	int sourceIndex = pos + 2 + xDataLen + i;
	int newCurrLen = currLen - (xDataLen - newDataLen); 
		
	while(destIndex < newCurrLen){
		arr[destIndex] = arr[sourceIndex];
		i++;
		destIndex = pos + 2 + newDataLen +i;
		sourceIndex = pos + 2 + xDataLen + i;
	}

	//Now, the shifted bytes should be rewritten to 0
	for(int i = newCurrLen; i<currLen; i++)
	      arr[i] = 0;		
	currLen = newCurrLen;
}

void rightShift(int pos, int newDataLen){
	//pos = index of id field of entry being overWritten
	//pos+1 = index of dataLen
	//pos+1+xDataLen is the end of entry
	//source address = currLen - i; destAddress = newCurrLen - i;
	//no of loops = number of bytes of data after current entry
	
	int i=0;
	int xDataLen;
	if(!(arr[pos+1] == 0))
		xDataLen = arr[pos+1] - '0'; //right shifting may occur while adding a new entry, at such time arr[pos+1] may be an integer
	else xDataLen = 0;	//here, arr[pos+1] is an integer, i.e it should be zero 

	int newCurrLen = 0;
	if(currLen == 0)
     		newCurrLen = currLen + (newDataLen + 2 - xDataLen); //while adding first element
	else newCurrLen = currLen + (newDataLen - xDataLen);

	int sourceIndex = currLen - i;
	int destIndex = newCurrLen -i;
	
	while(sourceIndex < currLen){
		printf("Dest Index: %d, Source Index : %d\n\n", destIndex, sourceIndex);
		arr[destIndex] = arr[sourceIndex];
		i++;
	}
	currLen = newCurrLen;	
}

void overWriteFunc(char id){
	int dataLen;
	int xDataLen;
	int remLen = MAX_ARR_LENGTH - currLen;//remaining number of empty bytes in the array

	//the last entry, if any, should have atleast 2 bytes of data, id and len(might as well be 0)
	for(int i =0; i<=currLen ; i ++){ 
		if(arr[i] == id){
			if(arr[i+1] == 0){
			       	xDataLen= 0; //when creating a new entry, then at this point,
			       		     //only an ID will be already there, and remaining 
					     //fields of the entry will have int 0 not a char
				remLen = remLen -2; // xDataLen == 0 during new entry, there we 
						    //must reserve space for dataLen and id bytes
			}
			else xDataLen = arr[i+1] - '0'; //coverting from character to integer

			while(1){ // keep iterating until a valid length of data is input
				if(remLen == 0){
					printf("There is only space for storing ID and DataLength for this entry. Setting dataLength to 0 \n\n");
					arr[i+1] = '0';
					currLen = MAX_ARR_LENGTH;
					return;
				}
				printf("Enter length of data:");
				scanf(" %d", &dataLen);
				clearInput();
				if(dataLen>remLen)
			        	printf("Data length is too long. You can add a maximim of %d bytes\n\n", remLen);
				else break;
			}
			//Now we have a valid data length, now if length is greater than previous length, then shift right
			//if previous length<new length, shift left
			if(xDataLen<dataLen)
				rightShift(i, dataLen);
			else if(xDataLen>dataLen) 
				leftShift(i, dataLen);
			
			printf("Checkpoint 1\n\n");
			//if same length, then no shifting required
			//now assign new length value to length field
			arr[i+1] = dataLen + '0'; //here dataLen is int but arr[i+1] is of type char

			//all set, now write data
			printf("Enter %d bytes of data:", dataLen);
			for(int j = 1; j<=dataLen; j++)
				scanf(" %c", &arr[i+1+j]); //arr[i+1+j} coz arr[i] = id, arr[i+1]=dataLen
			remLen = MAX_ARR_LENGTH - currLen;
			clearInput();
			return;
		}
		if(arr[i] != 0){
			i=i + 1 + arr[i+1] -'0'; //only look for bytes that are supposed to carry id
						//i will be incremented 1 more coz of for loop
		}
	}

	printf("Failed to write data. ID does not exist\n");
}


void storeNewData(){
	char overWrite;
	char id;
	int remLen = MAX_ARR_LENGTH - currLen;
	
	if(remLen == 0){//if there is no more space left in the array
		while(1){
			printf("Maximum storage limit reached. You can overwrite an existing entry. Proceed?");
			scanf("%c", &overWrite);
			clearInput();
		
			if(overWrite == 'y'){	
				id = inputId();						
				overWriteFunc(id);
				return;
			}
			else if(overWrite == 'n') return;
			//if none of the above, then iterates the loop		
			else printf("Wrong Input. Try again\n");	
		}
	}
	
	id = inputId();

	if(!validateID(id)){
		char overWrite;
		while(1){
			printf("ID already exists. Would you like to overwrite?:");
			scanf("%c", &overWrite);
			clearInput();
		
			if(overWrite == 'y'){				
				overWriteFunc(id);
				return;
			}
			else if( overWrite == 'n') return;
			
			//if none of the above, then iterates the loop		
			else printf("Wrong input. Try again\n");
		}
	}
	else{ //create a new node, update head, update count
		if(count == 0)
			arr[0] = id;
		else arr[currLen] = id;// a new entry will always be added to the end, coz the array is consolidated
		overWriteFunc(id);
		count++;		
		printf("\n");	
	}
}


void deleteData(){
	if(count == 0) {
		printf("Database is empty");
		return;
	}

	char id;
	int remLen = MAX_ARR_LENGTH - currLen;
	//exceptional cases when deleting last entry in the array, then there will be no left shifting
	//also, exception when deleting the only entry in the array, there also, no left shifting
	id = inputId();
	
	if(count == 1){ //if we are deleting the only entry, then there's no need of shifting,
			//only overWrite with 0
		int xDataLen = arr[1] - '0'; //dataLength of first entry
		for(int i =0; i<xDataLen; i++)
			arr[i] = 0;
		count  = 0;
		currLen = 0;
		remLen = MAX_ARR_LENGTH - currLen;
		return;
	}
	
	for(int i = 0 ; i < currLen ; i++){
		if(arr[i] == id){ 
			printf("Before Left shifting in delete Func\n\n");
			leftShift(i,-2);  //left shift function is defined for calling from overWriteFunc. to make it work here, we need to put -2 as newDataLen arg 
			count--;
			remLen = MAX_ARR_LENGTH - currLen;
			printf("Checkpoint 2\n\n");
			return;
		}
		if(arr[i] != 0)
			i = i+ 1 + arr[i+1] -'0'; 
	}
	printf("Failed to delete. ID does not exist\n\n");
}

void printArray(){
	for(int i =0; i<MAX_ARR_LENGTH; i++)
		printf("%c ", arr[i]);
}

void printData(){
	if(count == 0){
		printf("DataBase is empty\n\n");
		return;
	}

	char id;
	
	id = inputId();	
	for(int i =0; i< currLen; i++){
		if(arr[i] == id){
			int xDataLen = arr[i+1] - '0';
			for(int j = 0; j<xDataLen+2 ; j++)
				printf("%c ", arr[i+j]);
			printf("\n\n");	
			return;
		}	
		if(arr[0] != 0)
			i = i + 1 + arr[i+1] -'0';
	}
	printf("Could not print data. ID does not exist\n\n");
}


int main(){
	int ch;
	while(1){
		printf("Choose an operation from the following:\n");
		printf("1. Store new Data\n");
		printf("2. Delete existing data\n");
		printf("3. Print existing data\n");
		printf("4. Exit\n\n");

		while(1){
			printf("Enter your choice:");
			int retVal = scanf(" %d",&ch);
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
			case 2: deleteData();
				break;
			case 3: printData();
				break;
			case 4: exit(0);
			case 5: printArray();
				break;
			default: printf("Invalid Choice\n\n");
		}
	}
}
