#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/inotify.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <limits.h>

// Main method
int main(int argc, char **argv){

	int fd = inotify_init();
	char buf[sizeof(struct inotify_event) + NAME_MAX + 1];
	struct inotify_event *evp;

	if(fd < 0){
		perror("inotify_init(): ");
		return(-1);
	}

	int watchfd = inotify_add_watch(fd, argv[1],IN_MODIFY);
	// Error checking

	for(;;){
		printf("Looping\n");
		if((read(fd,buf,sizeof(buf))) < 0){
			printf("Read error! \n");
		}

		evp = (struct inotify_event *) buf;
		//printf("The length: %d\n", buf->len);
		if((buf->len) > 0){
			printf("Modified: %s\n", buf->name);
		}
	}

}
