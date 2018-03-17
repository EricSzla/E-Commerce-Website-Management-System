#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/inotify.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

#include "monitor.h"
#include "logger.h"

// Main method
int main(int argc, char **argv){

	int fd = inotify_init();
	char buf[sizeof(struct inotify_event) + NAME_MAX + 1];
	struct inotify_event *evp;

	if(fd < 0){
		perror("inotify_init(): ");
		return(-1);
	}

	int watchfd = inotify_add_watch(fd, MONITOR_PATH,IN_CREATE | IN_MODIFY | IN_DELETE);
	if(watchfd == -1){
		logError(getlogin(), "monitor.c: Failed to inotify_add_watch");
	}else{

		for(;;){
			//printf("Looping\n");
			if((read(fd,buf,sizeof(buf))) < 0){
				printf("Read error! \n");
			}

			evp = (struct inotify_event *) buf;
			if((evp->len) > 0){
				printf("Modified: %s\nUser: %s\n", evp->name, getlogin());
				char name[strlen(evp->name)+1];
				strcpy(name,evp->name);
				//const char *extenssion[strlen(evp->name)+2];
				const char *extenssion = &name[strlen(name)-3];
				//strcpy(extenssion,name[strlen(name)-3]);
				if(!strcmp(extenssion,"swp") == 0){
					printf("Logging\n");
					logEvent("Modified",evp->name,getlogin());
				}
			}
		}
	}

}
