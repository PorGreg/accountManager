#ifndef UTILS_H
#define UTILS_H

#include "types.h"

String *getStringFromUser();
String *stringInit(char *baseStr);
bool stringPush(String *string, char c);
bool stringMalloc(String *string, int meret);
bool stringCompare(String *str1, String *str2);
bool stringCopy(String *target, char *source);
char *toString(String *str);
void stringReplace(String *str, char *source);
void stringFree(String *string);
void clearInputBuffer();
void getActualDate(String *date);

#endif