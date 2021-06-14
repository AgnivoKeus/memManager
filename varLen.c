#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_ARR_LENGTH 20

int count =0;
char arr[MAX_ARR_LENGTH];
int currLen = 0;
int remLen = MAX_ARR_LENGTH;

void clearInput(){//it will clear of any extra characters from the input buffer
	int c = getchar();
	while(c!='\n' && c!= EOF)
		c = getchar();
}

char inputId(){
	char id;
	printf("Enter ID: ");
	scanf(" %c", &id);
	clearInput();
	printf("\n\n");
	return id;
}

bool validateId(char id){
	int i = 0;
	while(i<currLen){
		if(arr[i] != 0){
			if(arr[i] == id)
				return false;
			else i = i+ 2 + arr[i+1] - '0';
		}
	}
	return true;
}

void addEntry(char id){
	int dataLen;
		
	int remSpace = 0;
	int xDataLen = arr[currLen+1] -'0';
	if(xDataLen<0){ 
		xDataLen = 0;
		remSpace = remLen-2; //for a new entry, two bytes have to be reserved for 
	}
	else remSpace = remLen;

	arr[currLen] = id;
	while(1){ //keep iterating till a valid dataLen is given
		printf("Enter length of data: ");
		scanf(" %d", &dataLen);
		clearInput();
		if(dataLen<0){
			printf("Please enter valid data Length\n");
		}
		else if(dataLen > remSpace) //one byte each for id and dataLen
			printf("Not enough memory, you can only add %d bytes of data\n\n", remSpace);
		else break;
	}
	arr[currLen + 1] = dataLen + '0';//converting to char
	printf("Enter %d bytes of data\n", dataLen);

	for(int j = 1; j<=dataLen; j++){
		scanf(" %c", &arr[currLen+1+j]);
	}
	clearInput();
	currLen = currLen + dataLen+2;
	
	remLen = MAX_ARR_LENGTH - currLen;
	count++;
}

void deleteData(char id){
	int i = 0;
	while(i<currLen){
		if(arr[i] != 0){
			if(arr[i] == id){
			        int xDataLen = arr[i+1] -'0';
				
				int nextIdPos = i+1+xDataLen+1; //index of next entry ID
			
				int nextEntryLen = arr[nextIdPos+1] -'0';//dataLen of next entry
		
				int newCurrLen = currLen - (xDataLen + 2);//len of deleted entry							 
				int destIndex = i;//+counter
				int sourceIndex = nextIdPos;//+counter
				
				while(destIndex<newCurrLen){
					arr[destIndex] = arr[sourceIndex];
					destIndex++;
					sourceIndex++;
				}
				
				int j=destIndex;
				while(j<currLen){ //
				       	arr[j] = 0;
					j++;
				}

				currLen = newCurrLen;
				count--;
				remLen = MAX_ARR_LENGTH - currLen;
				return;
			}
			else i = i+ 2 + arr[i+1] - '0';
		}
	}
	printf("Failed to delete data. Id not in database.\n\n");
}

void overWriteFunc(char id){
	char ch;
	int i = 0;
	int xDataLen = 0;
	int remSpace = 0;
	while(i<currLen){
		if(arr[i] != 0){
			if(arr[i] == id){
				xDataLen = arr[i+1] -'0';
				remSpace = remLen + xDataLen;
				if(remSpace<0)
					remSpace = xDataLen;
				break;
			}
			else i = i+ 2 + arr[i+1] - '0';
		}
	}
	printf("You will be able tor store %d bytes of total data for id: %c.\n", remSpace, id);
	while(1){
		printf("Are you sure you wish to overwrite the existing data?\n");

		printf("Enter y for yes and n for no: ");
		scanf(" %c", &ch);
		if(ch == 'y'){
			deleteData(id);
			addEntry(id);
			break;
		}
		else if(ch == 'n') return;
		else printf("Please enter valid input\n");
	}
}

void overWriteFuncId(){
	char id = inputId();
	char ch;
	int i = 0;
	int xDataLen = 0;
	int remSpace = 0;
	while(i<currLen){
		if(arr[i] != 0){
			if(arr[i] == id){
				xDataLen = arr[i+1] -'0';
				remSpace = remLen + xDataLen;
				if(remSpace<=0)
					remSpace = xDataLen;
				break;
			}
			else i = i+ 2 + arr[i+1] - '0';
		}
	}

	printf("You will be able to store maximum %d bytes of total data for id: %c.\n", remSpace, id);
	while(1){
		printf("Are you sure you wish to overwrite the existing data?\n");
		printf("Enter y for yes and n for no: ");
		scanf(" %c", &ch);
		if(ch == 'y'){
			deleteData(id);
			addEntry(id);
			break;
		}
		else if(ch == 'n') return;
		else printf("Please enter valid input\n");
	}

}

void storeNewData(){
	int remLen = MAX_ARR_LENGTH - currLen;

	if(remLen <= 2){
		char ch;
		while(1){
		   printf("Memory full. Would you like to overWrite an existing data?\n");

		   printf("Enter your choice: ");
		   scanf(" %c", &ch);
		   printf("\n");

		   if(ch == 'y'){
			   overWriteFuncId();
			   return;
		   }
		   else if(ch == 'n')
			   return;
		   else printf("Please enter a valid choice\n\n");
		}
	}
	
	char id = inputId();

	if(count == 0){ //no validation required coz db empty
		addEntry(id); 
	}
	else{
	   if(!validateId(id)) //id already present
	   	overWriteFunc(id);
	   else addEntry(id);
	}
}


void printData(){
	int i = 0;
	char id = inputId();
	while(i<currLen){
		if(arr[i] != 0){
			if(arr[i] == id){
				
				int dataLen = arr[i+1] -'0';
				
				for(int j = 0; j<=1 + dataLen; j++)
					printf("%c ", arr[i+j]);
				printf("\n\n");
				return;
			}
			else i = i+ 2 + arr[i+1] - '0';
		}
	}
	printf("Failed to print data. Id not in database.\n\n");
}

void printArray(){
	for(int i = 0; i<MAX_ARR_LENGTH; i++)
		printf("%c ",arr[i]);
	printf("\n\n");
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
			case 2: deleteData(inputId());
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
