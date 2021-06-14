#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_COUNT 2

typedef struct dataStorage{
	char arr[5];
	struct dataStorage* next;
} dataStorage;

dataStorage* root = NULL;
dataStorage* head = NULL;
dataStorage* tracker = NULL;
int count = 0;

void clearInput(){
	int c = getchar();
	while(c!='\n' && c!= EOF)
		c = getchar();
}

bool validateID(char id){
	if(root == NULL) return true;
	tracker = root;
	for(int i = 0; i < count; i++){
		if(tracker->arr[0] == id) return false;
		if(tracker != head) tracker = tracker->next; //while tracker is not last node, proceed
		//if tracker is last node, loop will end
	}
	return true;
}

void overWriteFunc(char id){
	tracker= root;
	for(int i = 0; i !=count; i++){
		if(tracker->arr[0] == id){
			printf("Enter 4 bytes of data:\n");
			for(int i =1; i!=5; i++)
				scanf(" %c", &tracker->arr[i]);
			clearInput();	
			return;
		}
		tracker = tracker->next;
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
		
			if(overWrite == 'y'){	//at this point, tracker is at the node with the same id, so overwrite that node	
				printf("Enter ID:");
				scanf("%c", &id);
				printf("\n");
				clearInput();	
				overWriteFunc(id);
				return;
			}
			else if(overWrite == 'n') return;
			//if none of the above, then iterates the loop		
			else printf("Wrong Input. Try again\n");	
		}
	}
	
	printf("Enter an ID: ");
	scanf("%c", &id);
	clearInput();
	printf("\n");
	if(!validateID(id)){
		char overWrite;
		while(1){
			printf("ID already exists. Would you like to overwrite?:");
			scanf("%c", &overWrite);
			clearInput();
		
			if(overWrite == 'y'){	//at this point, tracker is at the node with the same id, so overwrite that node
				overWriteFunc(id);
				return;
			}
			else if( overWrite == 'n') return;
			
			//if none of the above, then iterates the loop		
			else printf("Wrong input. Try again\n");
		}
	}
	else{ //create a new node, update head, update count
		if(root== NULL){
		       	root = (dataStorage*)malloc(sizeof(dataStorage));
			head = root;
			head->next = NULL;
		}
		else{
			head->next = (dataStorage*)malloc(sizeof(dataStorage));
			head = head->next;
			head ->next = NULL;
		}
		count ++;
		head->arr[0] = id;

		printf("Enter 4 bytes of data:\n");
		for(int i =1; i!=5;i++)
			scanf(" %c", &head->arr[i]);
		clearInput();
		printf("\n");	
	}
}

void deleteData(){
	if(count == 0) {
		printf("Database is empty");
		return;
	}

	tracker = root;
	dataStorage* prev=root;
	char id;

	printf("Enter ID:");
	scanf("%c", &id);
	clearInput();
	
	//here tracker is at root
	//if root is the node to be deleted
	if(tracker->arr[0] == id){
		tracker = root->next;
		free(root);
		root = tracker;
		count --;
		return;
	}
	//here we have already checked root, so check from next node
	tracker = tracker->next;

	for(int i =1; i!= count; i++){
		if(tracker->arr[0]==id){
			prev->next = tracker->next;
			free(tracker);
			count --;
			return;
		}
		prev = tracker;
		if(tracker!= head) tracker = tracker->next;
		//else the loop will end
	}
	printf("Failed to delete. ID does not exist\n\n");
}

void printData(){
	tracker = root;
	char id;

	printf("Enter ID:");
	scanf("%c", &id);
	clearInput();

	for(int i =0; i!= count; i++){
		if(tracker->arr[0]==id){
			for(int i = 0; i !=5 ; i++)
				printf("%c ",tracker->arr[i]);
			printf("\n");
			return;
		}
		if(tracker!= head)
			tracker = tracker->next;
	}
	printf("Could not print data. ID does not exist\n\n");
}

int main(){
	int ch;
	while(1){
		printf("Choose an operation from the following:\n");
		printf("1. Store new Data\n");
		printf("2. Delete existing data\n");
		printf("3. Print existing data\n\n");
		
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
			default: printf("Invalid Choice");
		}
	}
}
