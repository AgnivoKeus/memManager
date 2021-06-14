    #include <stdio.h> 
    #include <stdlib.h> 
    #include <string.h> 
    int main(int argc, char *argv[]){ 
    	 
    	// Initializing the file pointer 
    	FILE *fs; 
     
    	char ch, buffer[32]; 
    	int i = 0, arr[100], j = 0; 
    	 
    	// Openning the file with file handler as fs 
    	fs = fopen("data.txt", "r"); 
     
    	// Read the file unless the file encounters an EOF 
    	while(1){ 
    		// Reads the character where the seeker is currently 
    		ch = fgetc(fs); 
     
    		// If EOF is encountered then break out of the while loop 
    		if(ch == EOF){ 
    			break; 
    		} 
     
    		// If the delimiter is encounterd(which can be 
    		// anything according to your wish) then skip the character 
    		// and store the last read array of characters in 
    		// an integer array 
    		else if(ch == ','){ 
     
    			// Converting the content of the buffer into 
    			// an array position 
    			arr[j] = atoi(buffer); 
     
    			// Increamenting the array position 
    			j++; 
     
    			// Clearing the buffer, this function takes two 
    			// arguments, one is a character pointer and  
    			// the other one is the size of the character array 
    			bzero(buffer, 32); 
     
    			// clearing the counter which counts the number 
    			// of character in each number used for reading 
    			// into the buffer. 
    			i = 0; 
     
    			// then continue 
    			continue; 
    		} 
    		else{ 
     
    			// reads the current character in the buffer 
    			buffer[i] = ch; 
     
    			// increamenting the counter so that the next 
    			// character is read in the next position in  
    			// the array of buffer 
    			i++; 
    		} 
    	} 
     
    	// printing out all the elements that are stored in the 
    	// array of integers 
    	for(i = 0; i < j; i++){ 
    		printf("Number [%d]: %d\n", i, arr[i]); 
    	} 
    } 