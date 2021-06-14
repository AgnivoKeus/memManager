#include <stdio.h>
#include <stdlib.h>

char arr[5];

void clearInput(){
    int c = getchar();
    while(c!= '\n' && c != EOF)
        c = getchar();
}

void saveFunc(void){
        printf("Enter 5 characters:\n");
        for(int i=0; i<5; i++){
            scanf(" %c", &arr[i]);
        }
        clearInput();
        printf("\n");
}

void clearFunc(void){
        for(int i=0; i<5; i++)
                arr[i] = '0';
                
        printf("Data cleared successfully\n\n");
}

void printFunc(void){
        for(int i =0; i<5; i++)
                printf("%c ", arr[i]);
        printf("\n\n");
}


int main(){
        int ch;
        while(1){
                printf("Please select an operation of your choice:\n");
                printf("1. Save 5 bytes\n");
                printf("2. Clear 5 bytes\n");
                printf("3. Print 5 bytes\n");
                printf("4. Exit\n\n");
                
                while(1){
                    printf("Enter your choice:");
                    int retVal = scanf("%d", &ch);
                    printf("\n");
                    if(retVal == 1 || retVal == EOF) break;
                    else if(retVal == 0){
                        printf("Bad Input\n");
                        clearInput();
                    } 
                }
                switch(ch){
                        case 1: saveFunc();
                                  break;
                        case 2 : clearFunc();
                                   break;

                        case 3 : printFunc();
                                   break;
                        case 4 : exit(0);
                                   break;
                        default: printf("Invalid choice\n");
                                 break;
                }
        }
        return 0;
}
