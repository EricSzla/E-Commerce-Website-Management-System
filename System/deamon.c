#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/dir.h>
#include <sys/param.h>

#include "backup.h" //  Library defined for backup
#include "deamon.h" // Library defined for deamon

// Define backup deamon
// The deamon will back-up the live version every night and 
// will initialize the start of uploading the intranet site to the live folder
// after it have finished with the back-up
static void back_up_deamon(){
	//printf("Setting up deamon...\n");
	time_t now;
	struct tm *nightTime;
	double seconds;
	//printf("deamon.c: setting time\n");
	time(&now);
	//printf("deamon.c: current time set\n");
	nightTime = localtime(&now);
	//printf("deamon.c: nightTime set\n");
	nightTime->tm_hour = 17;
	nightTime->tm_min = 40;
	nightTime->tm_sec = 0;

	//double test; 
	//test = difftime(now,mktime(nightTime));
	//printf("Time difference: %1.1f\n",test);
	pid_t pid;
	//printf("deamon.c: before 1st fork\n");
	pid = fork(); // Fork
	//printf("deamon.c: after 1st fork\n");
	if(pid < 0){
		printf("Deamon.c : Error pid < 0.\n");
		// Error have occured
		exit(EXIT_FAILURE);
	}

	if(pid > 0){
		// Parent process
		exit(EXIT_SUCCESS);
	}

	if(setsid() < 0){
		printf("Deamon.c: Error setsid() < 0\n");
		exit(EXIT_FAILURE);
	}

	pid = fork();
	if(pid > 0){
		exit(EXIT_SUCCESS);
	}else{
		//printf("Deamon.c: Starting deamon..\n");
		//file mode creation mask set to 0
		umask(0);
		// Change current working directory to root
		if(chdir("/") < 0){
			exit(EXIT_FAILURE);
		}

		// Close open file descriptors
		int x;
		for(x = sysconf(_SC_OPEN_MAX); x>=0; x--){
			close(x);
		}

		// Signal handler goes here.
		//
		// Log files goes here.
		//

		// Keep process alive with infinite loop
		while(1){
			sleep(1);
			time(&now);
			seconds = difftime(now,mktime(nightTime)); // difference between current time and nightTime (backUpTime) in seconds
			// If seconds == 0 means we can start the back-up.
			if(seconds == 0){
				char path[MAXPATHLEN];
				strcpy(path,LIVE_PATH);

				char dest[MAXPATHLEN];
				strcpy(dest,BACKUP_PATH);
				backup(path,dest); // Make a backup
				update(INTRANET_PATH,LIVE_PATH); // Then upload the  intranet 
			}
		}
	}
}

int main(){
	back_up_deamon();
	return 0;
}
