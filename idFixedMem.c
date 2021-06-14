#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_COUNT 10
#define MAX_ARR_LENGTH 50

char arr[MAX_ARR_LENGTH];
int count = 0;

void clearInput(){//it will clear of any extra characters from the input buffer
	int c = getchar();
	while(c!='\n' && c!= EOF)
		c = getchar();
}

char inputId(){
	char id;
	printf("Enter ID:");
	scanf("%c", &id);
	printf("\n");
	clearInput();	
	return id;
}

bool validateID(char id){//checks if an id is already in use, returns false if already in use
	if(count == 0) return true;
	for(int i =0; i <=MAX_ARR_LENGTH-5; i+=5){
		if(arr[i] == id)
			return false;
	}
	return true;
}


void overWriteFunc(char id){
	for(int i = 0; i<= MAX_ARR_LENGTH-5; i+=5){ //ids for each entry will be stored 
							//at every 5th position viz. 0, 5, 10 ..
		if(arr[i] == id){
			printf("Enter 4 bytes of data\n");
			for(int j =1; j<5; j++){
				int retVal = scanf(" %c", &arr[i+j]);	
			}
			clearInput();
			return;
		}
	}
	printf("Failed to write data. ID does not exist\n");
}


void storeNewData(){
	char overWrite;
	char id;
	printf("Count : %d\n\n", count);

	if(count == MAX_COUNT){
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
		else{
			for(int i=0; i<=MAX_ARR_LENGTH - 5; i +=5){
				if(arr[i] == NULL){ //checking if this position is empty
					arr[i] = id;
					break;
				}
			}
		}
		overWriteFunc(id);
		count++;		
//		clearInput();
		printf("\n");	
	}
}


void deleteData(){
	if(count == 0) {
		printf("Database is empty");
		return;
	}

	char id;

	id = inputId();		
	for(int i =0; i != MAX_ARR_LENGTH -5 ; i +=5){
		if(arr[i] == id){
			for(int j = 0; j<5 ; j++)
				arr[i+j] = NULL; //overWrite all positions of the id to 0
			count --;
			return;
		}
	}
	printf("Failed to delete. ID does not exist\n\n");
}


void printData(){
	if(count == 0){
		printf("DataBase is empty\n\n");
		return;
	}

	char id;
	
	id = inputId();	
	for(int i =0; i!= MAX_ARR_LENGTH -5; i+=5){
		if(arr[i] == id){
			for(int j =0; j<5 ; j++)
				printf("%c ", arr[i+j]);
			printf("\n\n");	
			return;
		}	
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
			int retVal = scanf("%d",&ch);
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
			default: printf("Invalid Choice\n\n");
		}
	}
}
