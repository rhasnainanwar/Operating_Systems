#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
	char bin; // for character by character reading
	int count = 0; // counter
	bool print = false; // if program should start printing
	int infoFile; // file to get information from
	
	printf("Welcome! You can find proc system information here!!!\n");
	
	// processor manufacturer and model
	printf("\nPROCESSOR INFO");
	infoFile = open("/proc/cpuinfo", O_RDONLY);
	
	if(!infoFile){
		perror("Error reading CPU information from proc!\n");
	}
	
	while(read(infoFile, &bin, 1) == 1){
		if(bin == '\n') count++;
		
		if(count == 1 || count == 4) print = true;
		if(count == 2) print = false;
		
		if(count == 5) break;
		
		if(print) printf("%c", bin);
	}
	
	// kernerl version
	printf("\n\nKERNEL INFO\n");
	infoFile = open("/proc/sys/kernel/osrelease", O_RDONLY);
	
	if(!infoFile){
		perror("Error reading kernel information from proc!\n");
	}
	
	printf("Version: ");
	while(read(infoFile, &bin, 1) == 1){
		if(bin == '\n') break;
		
		printf("%c", bin);
	}
	
	
	printf("\n\nTIME INFO\n");
	
	// Uptime
	infoFile = open("/proc/uptime", O_RDONLY);
	
	if(!infoFile){
		perror("Error reading kernel information from proc!\n");
	}
	
	printf("Time since last boot: ");
	while(read(infoFile, &bin, 1) == 1){
		if(bin == ' ') break;
		
		printf("%c", bin);
	}
	printf(" sec\n");
	
	// idle time
	printf("Idle Time: ");
	while(read(infoFile, &bin, 1) == 1){
		if(bin == '\n') break;
		
		printf("%c", bin);
	}
	printf(" sec\n");
	
	// user mode time
	infoFile = open("/proc/stat", O_RDONLY);
	
	if(!infoFile){
		perror("Error reading kernel information from proc!\n");
	}
	
	print = false;
	count = 0;
	
	printf("Time spent in User Mode:");
	while(read(infoFile, &bin, 1) == 1){
		if(bin == ' '){
			print = true;
			count++;
		}
		
		if(print)	printf("%c", bin);
		
		if(count == 3){
			print = false;
			break;
		}
	}
	printf("sec\n");
	
	// system mode time
	printf("Time spent in System Mode:");
	while(read(infoFile, &bin, 1) == 1){
		if(bin == ' ') count++;
		
		if(count == 4) print = true;
		
		if(print) printf("%c", bin);
		
		if(count == 5){
			print = false;
			break;
		}
	}
	printf("sec\n");
	
	
	printf("\n\nMEMORY INFO\n");	
	// configured memory
	
	infoFile = open("/proc/meminfo", O_RDONLY);
	
	if(!infoFile){
		perror("Error reading kernel information from proc!\n");
	}
	count = 0;
	print = true;
	while(read(infoFile, &bin, 1) == 1){
		if(bin == '\n') count++;
		
		if(count == 1) print = false;
		else if(count == 2) print = true;
		
		if(print) printf("%c", bin);
		
		if(count == 3) break;
	}
	
	return EXIT_SUCCESS;
}
