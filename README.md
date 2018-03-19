# E-Commerce-Website-Management-System

# Objectives

-	Create an internal Intranet site that is a duplicate copy of the live website. Staff can make changes to the Intranet version of the site and see the changes before it goes live. 
-	The website content should be backed up every night.
-	The changes made to the Intranet version of the site needs to be documented. The username of the user, the page they modified and the timestamp should be recorded.
-	The live site needs to be updated based on the changes made to the Intranet site. This should happen during the night. There are a large number of files on the website (5000+), only the files that have changed should be copied to the live site folder.
-	No changes should be allowed to be made to the site while the backup/transfer is happening.
-	If a change needs to be urgently made to the live site, it should be possible to make the changes. 

# Architecture

The project consists of two main background programs (daemons):

-	daemon.c + daemon.h:</br>The main purpose of daemon.c is to create a daemon that will handle the backup and upload of the modified files. 
-	monitor.c + monitor.h:</br>The main purpose of monitor.c is to create a daemon that will monitor any changes made to the intranet and document it in log/eventFile.txt

There are other C programs and headers that were developed in order to support the functionalities of the above daemons:

-	backup.c  + backup.h</br>Handles the backup and upload of the files, is called from daemon.c
-	logger.c + logger.h</br>Handles logging of events (e.g. modified file) and logging errors, saves the data in log/eventFile.txt or log/errorFile.txt
-	fileLocker.c + fileLocker.h</br>Handles permissions of the directories i.e. var/intranet and var/www. It locks the files during backup/upload and unlocks them after its finished.
-	timestamp.c + timestamp.h</br>Creates a string date, that can be used for logging or backup name
-	Makefile</br>Compiles the files

The high-architecture of the program as can be seen on figure 1, describes the relation between different files and how they can interact. 

The important aspect of it is to understand how deamon.c knows what to backup i.e. monitor.c monitors the changes in var/intranet and logs those changes (along with username, timestamp, file name) in log/logEvent.txt. 

When deamon.c wants to upload the modified files to the live version (every night or urgentUpload()) then it goes to logger, which fetches the names of the files that have been added/modified, once the names have been fetched it can call upload from backup.c, which will handle the copying of the data.

![Figure1](https://user-images.githubusercontent.com/15609881/37626750-1d052926-2bc9-11e8-9ea3-1efec25537c1.png)
Figure 1: System Architecture.

# Setting Up

In order to proceed with compiling and running the program the paths have to be set accordingly to the machine running the program. The backup is set to execute at 23:59 (11:59PM), it can be configured, by setting the time by following the instructions below:

## Setting the time:

### daemon.c
In order to change the time the back_up_daemon() function have to be modified.

``` C

static void back_up_daemon(){
	time_t now;
	struct tm *nightTime;
	double seconds;
	time(&now);
	nightTime = localtime(&now);
	nightTime->tm_hour = 23;
	nightTime->tm_min = 59;
	nightTime->tm_sec = 59;

	pid_t pid;
 ```
 
Adjust the time accordingly.

``` C
nightTime -> tm_hours = (DESIRED HOUR FOR EXECUTION)
nightTime -> tm_min = (DESIRED MINUTES FOR EXECUTION)
nightTime-> tm_sec = (DESIRED SECONDS FOR EXECUTION)
```

## Paths to change:

### daemon.h
The daemon.h stores pre-defined paths:

```C
#ifndef DEAMON_H
#define DEAMON_H

#define LIVE_PATH "/home/eryk/College/SystemsSoftware/E-Commernce-Website-Management-System/var/www"
#define BACKUP_PATH "/home/eryk/College/SystemsSoftware/E-Commernce-Website-Management-System/var/backup/"
#define INTRANET_PATH "/home/eryk/College/SystemsSoftware/E-Commernce-Website-Management-System/var/intranet/html/"

void fileUpload();

#endif
```

The following paths have to be changed, and full path have to be provided to point to the:

```C
LIVE_PATH		- path to var/www
BACKUP_PATH	- path to var/backup/
INTRANET_PATH	- path to var/intranet/html/
```

### logger.h
Stores paths to the log files, where events and errors will be logged i.e.

```C
ERROR_PATH – path to log/errorFile.txt
EVENT_PATH – path to log/eventFile.txt (modified files)
```

The standard error_path and event_path is defined as follow:

```C
#ifndef LOGGER_H
#define LOGGER_H

#define ERROR_PATH "/home/eryk/College/SystemsSoftware/E-Commernce-Website-Management-System/log/errorFile.txt"
#define EVENT_PATH "/home/eryk/College/SystemsSoftware/E-Commernce-Website-Management-System/log/eventFile.txt"
#define DATELEN 50
int logError(char *a, char *b);
int logEvent(char* a, char* b, char* c);
const char* getFileNames();
int isAdded(char *l, const char **b, int ctr);

#endif
```
### monitor.h
Last path to be modified, this is the actual path to the directory that will be monitored for changes
```C
MONITOR_PATH – path to var/intranet/html/
```

# Compiling

Depending from machine settings, the program may require to be run as root.
In order to compile the file the following commands have to be used:
```
make
make monitor
```
Expected output:

![make](https://user-images.githubusercontent.com/15609881/37627084-b0635d0e-2bca-11e8-8261-d9d3dd1fd5dc.png)

# Running

In order to run the program:
-	run monitor using:
``./monitor`` 
-	Check if the program is running using
``ps –aux | grep ./monitor``
-	run daemon using:
``./daemon``
-	check if the program is running using (as can be seen on Figure 8):
``ps -aux | grep ./daemon``

# After Running

## Monitoring files

The monitor daemon will monitor any changes done to intranet/html, therefore for demonstration purposes if we navigate to var/intranet/html and make the following changes:

-	create a file called fetchUserInformation.js
-	modify a file called index.html 

we can achieve that using:
``nano fetchUserInformation.js``
``nano index.html``

After the modifications made to intranet, the monitor daemon will be notified and will log these actions in the ``log/eventFile.txt``.

![cat](https://user-images.githubusercontent.com/15609881/37627188-2e30d5fe-2bcb-11e8-8e5d-6b68d2b874f7.png)

## Backup and update

After the daemon.c program is run the backup will automatically execute at 23:59, the time can be changes as described in setting up chapter of this document.

At the specified time the daemon will **lock the files** in var directory and make a backup of the www directory, the backup path is defined in daemon.h and is set to: ``var/backup/``. 

The backup is made with timestamp (date) as its name. 
```terminal
ls
18_03_18
```

The daemon.c program, after the backup has been finished has also updated the ``www/html`` with only the **modified files** that have been logged in ``log/eventFile.txt``. 

So after modyfing the fetchUserInformation.js and index.html the ``www/html`` directory should look like:
```terminal
ls
fetchUserInformation.js   index.html
```

# Urgent Upload

In order to force the daemon to make an urgent upload of the intranet files to the live path use the kill command:

``` cmd
kill –USR1 pid
```
example usage:

``` cmd
user@debian: ps -aux | pgrep ./daemon
2684
user@debian: kill -USR1 2684
user@debian:
```

When ``kill –USR1`` command is received by ``signal_handler`` in ``daemon.c``, it will call a function to proceed with the upload to live version from intranet. 

In order to demonstrate this functionality if we add a file in ``/var/intranet/html`` called **urgentUpload.txt** and call ``kill –USR1`` on the daemon PID, then the **urgentUpload.txt** will be successfully added to the live path: ``var/www/html``.




