#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/inotify.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/dir.h>
#include <sys/param.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

#include "monitor.h"
#include "logger.h"

int monitor_daemon();

// Main method
int main(int argc, char **argv){
	monitor_daemon();
}

int monitor_daemon(){
	pid_t pid;
	pid = fork(); // Fork
	// Error checking
	if(pid < 0){
		logError(getlogin(),"monitor.c: (pid < 0)");
		exit(EXIT_FAILURE);
	}else if(pid > 0){
		// Kill Parent process
		exit(EXIT_SUCCESS);
	}

	if(setsid() < 0){
		logError(getlogin(),"monitor.c: (setsid() < 0");
	}

	pid = fork();
	if(pid > 0){
		// Kill parent
		exit(EXIT_SUCCESS);
	}else{
		// Starting the deamon
		umask(0); // file creation mask
		if(chdir("/") < 0){
			logError(getlogin(),"monitor.c: (chdir error)");
			exit(EXIT_FAILURE);
		}
		// Close open file descriptors
		int x;
		for(x = sysconf(_SC_OPEN_MAX); x>=0; x--){
			close(x);
		}

		// Initialize notifier
		int fd = inotify_init();
		char buf[sizeof(struct inotify_event) + NAME_MAX + 1];
		struct inotify_event *evp;
		// Error checking for notifier
		if(fd < 0){
			logError(getlogin(),"monitor.c: (inotify_init())");
			return(EXIT_FAILURE);
		}
		// Add pre-defined path to be monitored for changes e.g. intranet (can be found in monitor.h)
		int watchfd = inotify_add_watch(fd, MONITOR_PATH,IN_CREATE | IN_MODIFY);
		if(watchfd == -1){
			logError(getlogin(), "monitor.c: Failed to inotify_add_watch");
			exit(EXIT_FAILURE);
		}else{

			for(;;){
				printf("Looping\n");
				if((read(fd,buf,sizeof(buf))) < 0){
					logError(getlogin(),"monitor.c: (Read error)");
				}
				// set struct
				evp = (struct inotify_event *) buf;
				if((evp->len) > 0){
					printf("Modified: %s\nUser: %s\n", evp->name, getlogin());
					char name[strlen(evp->name)+1];
					strcpy(name,evp->name);
					const char *extenssion = &name[strlen(name)-3];
					if(!strcmp(extenssion,"swp") == 0){
						printf("Logging\n");
						// File have been modified (or created), add it to the event log file 
						logEvent("Modified",evp->name,getlogin());
					}
				}
			}
		}
	}
}
