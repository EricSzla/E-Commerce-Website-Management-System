CC = gcc

objects = objFiles/deamon.o objFiles/backup.o objFiles/fileLocker.o objFiles/timestamp.o objFiles/logger.o
cfiles = deamon.c backup.c fileLocker.c timestamp.c
headers = deamon.h backup.h fileLocker.h timestamp.h logger.h monitor.h
monitorHeaders = monitor.h logger.h
monitorFiles = monitor.c logger.c
monitorObjects = monitor.o logger.o

name = deamon
DIR_OBJ = objFiles

deamon: $(objects)
	$(CC) -o $(name) $(objects) -lm

$(DIR_OBJ)/deamon.o: deamon.c $(headers)
	$(CC) -c deamon.c -o $(DIR_OBJ)/deamon.o

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
