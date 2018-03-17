#ifndef LOGGER_H
#define LOGGER_H

#define ERROR_PATH "/home/eryk/College/SystemsSoftware/Assignment/log/errorFile.txt"
#define EVENT_PATH "/home/eryk/College/SystemsSoftware/Assignment/log/eventFile.txt"

int logError(char *a, char *b);
int logEvent(char* a, char* b, char* c);
const char* getFileNames();
int isAdded(char *l, const char **b, int ctr);

#endif
