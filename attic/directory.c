#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

// defines
#define PARAMETER_OFFSET 2
#define MAX_PATH 1024

// prototypes
char* printDirectory(char* input);

// global variables
int minFileSize;

/**
 * Entry point for the program.
 */
int main(int argc, char* argv[]) {
	// leave if improper input
	if (argc == 1) {
		printf("Error: No parameters.");
		return 1;
	}

	// set to global variables to sane values
	minFileSize = atoi(argv[1]);

	// parse the command line to determine directories
	printf("Using the following directories:\n");
	int current_dir = 0;
	int end_dir = argc - 2;
	char temp[256];
	while (current_dir < end_dir) {
	 	// use those directories
		sprintf(temp, "\t%s\n", argv[current_dir+2]);
		printf(temp);

		// otherwise use . by default
		current_dir++;
		
		// recurse through the directories
		printDirectory(argv[current_dir+1]);
	}

	return 0;
}

/**
 * Recursive function for printing out 
 */
char* printDirectory(char* directory) {
	// open the directory stream
	DIR* dirStreamPtr = opendir(directory);
	if (dirStreamPtr == NULL) {
		perror("Error: While opening directory stream.\n");
		exit(1);
	}

	// read through the directory stream
	struct dirent* direntStruct = NULL;
	struct stat buf;
	char currentFile[MAX_PATH];
	readdir(dirStreamPtr);
	readdir(dirStreamPtr);
	while (direntStruct = readdir(dirStreamPtr)) {
		// get stat information
		sprintf(currentFile, "%s%c%s", directory, '/', direntStruct->d_name);
		if (stat(currentFile, &buf) == -1) {
			perror("Error:");
			exit(1);
		}
		
		// for each directory print out if size is above minFileSize
		if (S_ISDIR(buf.st_mode) != 0 && buf.st_size > minFileSize) printf("(DIR)");

		// for each file print out if size is above minFileSize
		if (buf.st_size > minFileSize) printf("\t\t%s %u\n", direntStruct->d_name, buf.st_size);
		
		// for each directory recurse
		if (S_ISDIR(buf.st_mode) != 0) printDirectory(currentFile);
	}

	// close the directory stream
	if (closedir(dirStreamPtr) == -1) {
		perror("Error: While closing directory stream.\n");
		exit(1);
	}
}
