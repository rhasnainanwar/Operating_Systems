#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>

#define BUF_SIZE 8192

int main(int argc, char* argv[]) {

	int input_file; /* Input file descriptors */
	ssize_t ret_in; /* Number of bytes returned by read()*/
	char buffer[BUF_SIZE]; /* Character buffer */
	bool found = false;
	char * term;
	int termLen;

	/* Are complete arguments missing */
	if(argc < 3){
		printf("Usage: a.out term file1\n");
		return 1;
	}

	term = argv[1];
	for(termLen = 0; term[termLen] != '\0'; termLen++);

	input_file = open(argv[2], O_RDONLY);
	if(input_file == -1){
		perror(argv[2]);
		return 2;
	}

	/* Reading the file */
	while((ret_in = read(input_file, &buffer, BUF_SIZE)) > 0){
		int len = (int)ret_in;

		for(int i = 0; i < len; i++){

			if(term[0] != buffer[i])
				continue;

			int j;
			for(j = 1; j < termLen; j++)
				if(term[j] != buffer[i+j])
					break;

			if(j == termLen){
				found = true;
				int k;
				printf("\"...");

				if(i - 20 < 0)
					k = 0;
				else
					 k = i - 20;

				for(; k < i + j + 20 && k < len; k++)
					if(buffer[k] != '\n')
						printf("%c", buffer[k]);
				printf("...\"\n");

				i = i + termLen - 1;
			}
		}
	}

	if (!found)
	{
		printf("The term does not exist in the file: %s\n", argv[2]);
	}

	/* close file */
	close(input_file);

	return(EXIT_SUCCESS);
}
