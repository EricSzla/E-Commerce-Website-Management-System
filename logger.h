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
