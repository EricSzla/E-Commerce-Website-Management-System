#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include "backup.h"
#include "fileLocker.h"
#include "timestamp.h"

int isLocked;

// Function that make a back-up
// Takes two parameters the source and target
// I.e. what to copy and where to copy
void backup(char * sourceFile, char * targetFile){
	lockFiles(sourceFile); // locks the files during backup
	isLocked = 1;
	// Create timestamp
	char date[DATELEN];
	char *buff_time = date_type_string(date); // Append timestamp to filename 
	strcat(targetFile, buff_time);

	char *args[] = {"cp","-R", sourceFile, targetFile, NULL}; // args for copy command e.g. cp -R source.txt dest.txt
	execv("/bin/cp",args); // execute and pass path to cp command and args
	printf("Back_up complete\n");
	unlockFiles(sourceFile); // Unlocks the file after backup
	isLocked = 0;
}

// Funciton used to update the files in live folder
void update(char * sourceFile, char* targetFile){
	lockFiles(sourceFile);
	isLocked = 1;
	char *args[] = {"cp", "-R", sourceFile, targetFile, NULL};
	execv("/bin/cp", args);
	printf("Live version updated.\n");
	unlockFiles(sourceFile);
	isLocked = 0;
}

int getFilesLocked(){
	return isLocked;
}
