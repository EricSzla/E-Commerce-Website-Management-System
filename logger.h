#ifndef LOGGER_H
#define LOGGER_H

#define ERROR_PATH "log/errorFile.txt"
#define EVENT_PATH "log/eventFile.txt"

int logError(char *a, char *b);
int logEvent(char* a, char* b, char* c);

#endif
