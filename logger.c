#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <sys/stat.h>

#include "logger.h"

int logError(char *usr, char *msg){
	FILE *pFile;
	char buffer[256];
	pFile = fopen(ERROR_PATH,"r+");
	if(pFile == NULL){
		return -1;
	}
	char mode[] = "0777";
	char buff[100] = ERROR_PATH;
	int i = 0;
	i = strtol(mode,0,8);
	if(chmod(buff,i) < 0){
		return -1;
	}
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
			exit(1);
		}
		// Write to a file
		fseek(pFile,0,SEEK_END);
		fprintf(pFile, "\nEvent: %s",event);
		fprintf(pFile, "\nFile: %s", file);
		fprintf(pFile, "\nUser: %s",user);
		fclose(pFile);
	}
}
