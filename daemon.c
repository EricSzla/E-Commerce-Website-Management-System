#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/dir.h>
#include <sys/param.h>

#include "backup.h" //  Library defined for backup
#include "daemon.h" // Library defined for deamon
#include "logger.h" // Library used for logging
// Fxn prototype
static void back_up_daemon();

void sig_handler(int sig){
	if(sig == SIGUSR1){
		printf("Handling SUGUSR1\n");
		fileUpload();
	}
}

int main(){
	back_up_daemon();
	return 0;
}

// Define backup deamon
// The deamon will back-up the live version every night and 
// will initialize the start of uploading the intranet site to the live folder
// after it have finished with the back-up
static void back_up_daemon(){
	time_t now;
	struct tm *nightTime;
	double seconds;
	time(&now);
	nightTime = localtime(&now);
	nightTime->tm_hour = 18;
	nightTime->tm_min = 35;
	nightTime->tm_sec = 0;

	pid_t pid;
	pid = fork(); // Fork
	//printf("deamon.c: after 1st fork\n");
	if(pid < 0){
		//printf("Deamon.c : Error pid < 0.\n");
		logError(getlogin(),"Deamon.c: (pid<0)");
		// Error have occured
		exit(EXIT_FAILURE);
	}

	if(pid > 0){
		// Parent process
		//printf("\nKilling the parent.\n");
		exit(EXIT_SUCCESS);
	}

	if(setsid() < 0){
		logError(getlogin(),"Deamon.c: (Error setsid() < 0)");
		exit(EXIT_FAILURE);
	}

	pid = fork();
	if(pid > 0){
		//printf("\nStarting monitor deamon.\n");
		exit(EXIT_SUCCESS);
	}else{
		//printf("\nDeamon.c: Starting deamon..\n");
		//file mode creation mask set to 0
		umask(0);
		//printf("Mask set.\n");
		// Change current working directory to root
		if(chdir("/") < 0){
			logError(getlogin(),"Deamon.c: (Failed to change dir!)");
			exit(EXIT_FAILURE);
		}

		// Close open file descriptors
		int x;
		for(x = sysconf(_SC_OPEN_MAX); x>=0; x--){
			close(x);
		}
		// Signal handler
		if(signal(SIGUSR1, sig_handler) == SIG_ERR){
			logError(getlogin(),"deamon.c: (SIGUSR1 Error)");
		}
		// Keep process alive with infinite loop
		while(1){
			//printf("Looping..\n");
			sleep(1);
			time(&now);
			seconds = difftime(now,mktime(nightTime)); // difference between current time and nightTime (backUpTime) in seconds
			//printf("Time: %1.1f\n",seconds);
			// If seconds == 0 means we can start the back-up.
			if(seconds == 0){
				char path[MAXPATHLEN];
				strcpy(path,LIVE_PATH);
				char dest[MAXPATHLEN];
				strcpy(dest,BACKUP_PATH);
				backup(path,dest); // Make a backup
				printf("Uploading files..");
				fileUpload();	   // Upload the modified files

			}
		}
	}
}

// Function to upload files to LIVE version
void fileUpload(){
	const char* fileNames[5000]; // stores file names
	char intranet_path[256];	     // initial path to intranet
	int i = 0;
	// Receive file names
	*fileNames = getFileNames();
	while(fileNames[i] != NULL){
		printf("Updating file: %s\n",fileNames[i]);
		strcpy(intranet_path, INTRANET_PATH); // Copy the pre-defined initial path to a variable
		strcat(intranet_path, fileNames[i]); // Add file name to the path, so it can be copied
		printf("Intranet_path: %s",intranet_path);
		// Clear up the end of the path
		strtok(intranet_path,"\n");
		// add "html" to path
		char live_path[256];
		strcpy(live_path, LIVE_PATH);
		strcat(live_path,"/html");
		update(intranet_path, live_path);    // Update the live version
		i++;
	}
}
