CC = gcc

objects = objFiles/deamon.o objFiles/backup.o objFiles/fileLocker.o objFiles/timestamp.o
cfiles = deamon.c backup.c fileLocker.c timestamp.c
headers = deamon.h backup.h fileLocker.h timestamp.h

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

clean:
	rm $(name) $(objects)
