#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <stdio.h>

#include "types.h"

char *readLine(FILE *file);
User *readUsers(AppState *appState);
void readTransactions(AppState *appState);
User *getUserDataFromFile(AppState *appState);
FILE *openFile(AppState *appState, char *fileName, char *mode);
void saveAppData(AppState *appState);
void resetAppData(AppState *appState);

#endif