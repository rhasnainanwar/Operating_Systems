#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>

#define BUF_SIZE 100

int main(int argc, char* argv[]) {
	int input_file; /* Input file descriptors */
	ssize_t ret_in; /* Number of bytes returned by read()*/
	char buffer[BUF_SIZE]; /* Character buffer */
	int n; /* Lines to print */
	int count = 0; /* Line break counter */

	/* Are complete arguments missing */
	if(argc < 3){
		printf("Usage: a.out n file\n");
		return 1;
	}

	n = atoi(argv[1]);

	input_file = open(argv[2], O_RDONLY);
	
	if(input_file == -1){
		perror(argv[2]);
		return 2;
	}

	off_t chars = lseek(input_file, 0, SEEK_END); // total characters in file
	
	if(chars == -1)
		perror("seek failed!");
	else {
		int pos = (int)chars;
		while(pos){
			if(lseek(input_file, --pos, SEEK_SET) != -1){
				char bin;
				if( (read(input_file, &bin, 1)) > 0 && bin == '\n') // count line breaks
					if(count++ == n)	break;
			}
			else	perror("seek failed!");
		}
		
		printf("Printing last %d lines:\n", n); // continue onwards where last line break was
		int past;
		while( (past = read(input_file, &buffer, BUF_SIZE)) == BUF_SIZE )
			printf("%s", buffer);
		if(past > 0){
			char last[past];
			lseek(input_file, 0-past, SEEK_CUR);
			read(input_file, &last, past);
			printf("%s\n", last);
		}
	
	}
	
	/* close file */
	close(input_file);

	return(EXIT_SUCCESS);
}
