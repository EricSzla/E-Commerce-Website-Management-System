#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

#include "fileLocker.h"

// Funciton to lock access to files
// Takes path to the files as paremeter
int lockFiles(char* location){
	int status;
	struct stat buffer;
	struct stat stat_struct;

	mode_t mode;
	stat(location, &stat_struct);  // Function call to determine infomation about a file 
	mode = stat_struct.st_mode & 33188; // set permissions
	mode &= ~(S_IRUSR);
	mode |= S_IXUSR;

	chmod(location,mode);  // chmod given mode
	status = stat(location,&buffer);
	printf("Locking: %s\n", location);
	return status;
}

int unlockFiles(char* location){
	int status;
	struct stat buffer;
	struct stat stat_struct;

	mode_t mode;
	stat(location, &stat_struct);
	mode = stat_struct.st_mode & 07777; // set permissions 
	mode &= ~(S_IRUSR);
	mode |= S_IXUSR;

	chmod(location,mode);
	status = stat(location, &buffer);

	printf("Unlocking: %s\n", location);
	return status;
}

