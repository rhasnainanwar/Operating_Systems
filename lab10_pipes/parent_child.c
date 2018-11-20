#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define STRLEN 100

int main() {
	int fd1[2]; // pipe 1
	int fd2[2]; // pipe 2
	
	pid_t process; // child process identifier
	
	if(pipe(fd1) == -1) {
		fprintf(stderr, "Pipe1 failed!");
		return -1;
	}
	
	if(pipe(fd2) == -1) {
		fprintf(stderr, "Pipe2 failed!");
		return -1;
	}
	
	
	process = fork(); // creating child
	
	if(process < 0) {
		fprintf(stderr, "fork Failed" ); 
        return -1; 
	}
	else if(process > 0) { // Parent
		close(fd1[0]); // close reading end before writing
		
		// writing
		char p2c[] = "Hello child, what's up?";
		write(fd1[1], p2c, sizeof(p2c)+1);
		close(fd1[1]); // close writing
		
		wait(NULL); // wait for child to send data
		
		// reading
		close(fd2[1]); // close writing
		char response[STRLEN];
		read(fd2[0], response, STRLEN); // read from child
		printf("Child: %s\n", response);
		close(fd2[0]); // close reading
	}
	else { // Child
		close(fd1[1]); // close wrting end before reading
		
		// reading
		char response[STRLEN];
		read(fd1[0], response, STRLEN); // read from Parent
		printf("Parent: %s\n", response);
		
		// Close both reading ends 
        close(fd1[0]); 
        close(fd2[0]);
        
        // writing
		char c2p[] = "Hey Papa!!!";
		write(fd2[1], c2p, sizeof(c2p)+1);
		close(fd2[1]); // close writing
		
		exit(0);
	}
	
	return EXIT_SUCCESS;
}
