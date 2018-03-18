CC = gcc

objects = objFiles/daemon.o objFiles/backup.o objFiles/fileLocker.o objFiles/timestamp.o objFiles/logger.o
cfiles = daemon.c backup.c fileLocker.c timestamp.c
headers = daemon.h backup.h fileLocker.h timestamp.h logger.h monitor.h
monitorHeaders = monitor.h logger.h
monitorFiles = monitor.c logger.c
monitorObjects = monitor.o logger.o

name = daemon
DIR_OBJ = objFiles

daemon: $(objects)
	$(CC) -o $(name) $(objects) -lm

$(DIR_OBJ)/daemon.o: daemon.c $(headers)
	$(CC) -c daemon.c -o $(DIR_OBJ)/daemon.o

$(DIR_OBJ)/backup.o: backup.c $(headers)
	$(CC) -c backup.c -o $(DIR_OBJ)/backup.o

$(DIR_OBJ)/fileLocker.o: fileLocker.c $(headers)
	$(CC) -c fileLocker.c -o $(DIR_OBJ)/fileLocker.o

$(DIR_OBJ)/timestamp.o: timestamp.c $(headers)
	$(CC) -c timestamp.c -o $(DIR_OBJ)/timestamp.o

monitor: $(monitorObjects)
	$(CC) -o monitor $(monitorObjects) -lm

objFiles/monitor.o: monitor.c $(headers)
	$(CC) -c monitor.c -o objFiles/monitor.o

objFiles/logger.o: logger.c $(headers)
	$(CC) -c logger.c -o objFiles/logger.o

clean:
	rm $(name) $(objects)
