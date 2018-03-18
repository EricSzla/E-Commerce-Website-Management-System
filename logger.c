#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <sys/stat.h>

#include "logger.h"
#include "timestamp.h"

int logError(char *usr, char *msg){
	FILE *pFile;
	char buffer[256];
	char mode[] = "0777";
	char buff[100] = ERROR_PATH;
	int i = 0;

	i = strtol(mode,0,8);
	if(chmod(buff,i) < 0){
		return -1;
	}

	pFile = fopen(ERROR_PATH,"r+");
	if(pFile == NULL){
		return -1;
	}
	fseek(pFile,0,SEEK_END);
	fprintf(pFile,"User: %s\n",usr);
	fprintf(pFile,"Msg: %s\n",msg);
	fclose(pFile);
}

int logEvent(char *event, char* file, char* user){
	FILE *pFile;
	char buffer[256];

	pFile = fopen(EVENT_PATH,"r+");
	if(pFile == NULL){
		logError(getlogin(),"Error Opening A File");
		return -1;
	}else{
		char mode[] = "0777";
		char buff[100] = EVENT_PATH;
		int i = 0;
		i = strtol(mode,0,8);
		if(chmod(buff,i) < 0){
			logError(getlogin(),"Error setting permissions");
			return -1;
		}
		// Create a timestamp
		fseek(pFile,0,SEEK_END);
		char date[DATELEN];
		char *buff_t = date_type_string(date);
		printf("Writing to a file\n");
		// Write to a file
		fprintf(pFile, "\nEvent: %s",event);
		fprintf(pFile, "\nFile: %s", file);
		fprintf(pFile, "\nUser: %s",user);
		fprintf(pFile, "\nTimestamp: %s", buff_t);
		fclose(pFile);
	}
}

const char* getFileNames(){
	//printf("Getting file names..\n");
	FILE *pFile;
	char buffer[256];

	//printf("Setting permissions\n");
	char mode[] = "0777";
	char buff[100] = EVENT_PATH;
	int i = 0;
	i = strtol(mode,0,8);
	if(chmod(buff,i) < 0){
		//printf("Cannot set permissions");
		logError(getlogin(),"Error setting permissions");
		return "Error";
	}

	pFile = fopen(EVENT_PATH,"r+");
	if(pFile == NULL){
		//printf("FILE IS NULL!!\n");
		logError(getlogin(),"logger.c(getFiles): cannot open file");
		return "Error";
	}

	//printf("Before while loop\n");
	const char *fileNames[5000];
	int counter = 0;

	while(1){
		char line[255];
		fgets(line,255,pFile);
		if(feof(pFile)){
			break;
		}
		if(counter >= 5000){
			break;
		}
		char type[255];
		strncpy(type,line,5);
		//printf("Type: %s\n", type);

		if(strcmp(type,"File:") == 0){
			//printf("\nFile Type!\n");
			// Means we are dealing with file name
			// Check if this file is not already added
			char path[256];
			strcpy(path,&line[6]); // remove "File:" tag
			if(isAdded(path, fileNames,counter) == 0){
				fileNames[counter] = path;
				printf("FileName[%d]: %s",counter, fileNames[counter]);
				counter++;
			}
		}
	}
	fclose(pFile);

	return *fileNames;
}

// Function used to check if a file name that have been stored in the eventFile.txt have been already
// added to the fileNames
// If it has been then it will return 1, otherwise will return 0
// The function is useful to increase performance on uploading the data to LIVE version,
// as it avoids duppplicate uploads
int isAdded(char *line, const char **fileNames, int ctr){
	int i;
	for(int i=0; i < ctr;i++){
		if(strcmp(fileNames[i],line) == 0){
			return 1;
		}
	}

	return 0;
}
