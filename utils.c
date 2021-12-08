#include "utils.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "types.h"

/**
 * @brief Get a String from the user
 *
 * @return String*
 */
String *getStringFromUser() {
    String *string = stringInit(NULL);
    if (string == NULL) {
        return NULL;
    }
    char c;
    while ((c = getchar()) != '\n' && c != EOF) {
        if (!stringPush(string, c))
            return NULL;
    }
    return string;
}

/**
 * @brief Initialize a String object
 *
 * @param baseStr base string you want to place in the new String
 * @return String*
 */
String *stringInit(char *baseStr) {
    String *string = (String *)malloc(sizeof(String));
    bool mallocResult = stringMalloc(string, 0);
    if (!mallocResult)
        return NULL;
    if (baseStr != NULL)
        stringCopy(string, baseStr);
    return string;
}

/**
 * @brief Push a char to a String object
 *
 * @param string
 * @param c
 * @return true
 * @return false
 */
bool stringPush(String *string, char c) {
    int newSize = string->size + 1;
    char *newData = (char *)malloc(newSize * sizeof(char));
    if (newData == NULL)
        return false;
    strcpy(newData, string->data);
    free(string->data);
    newData[string->size - 1] = c;
    newData[string->size] = '\0';
    string->data = newData;
    string->size = newSize;
    return true;
}

/**
 * @brief Allocate memory for a new String
 *
 * @param string
 * @param size
 * @return true
 * @return false
 */
bool stringMalloc(String *string, int size) {
    // size is given size + 1 because of the closing \0
    string->size = size + 1;
    string->data = (char *)malloc(string->size * sizeof(char));
    // put the closing \0 to the last character
    string->data[string->size - 1] = '\0';
    return string->data != NULL;
}

/**
 * @brief Compare two String objects
 *
 * @param str1
 * @param str2
 * @return true
 * @return false
 */
bool stringCompare(String *str1, String *str2) {
    return strcmp(str1->data, str2->data) == 0;
}

/**
 * @brief Copy the given string to the target String object
 *
 * @param target
 * @param source
 * @return true
 * @return false
 */
bool stringCopy(String *target, char *source) {
    int newSize = target->size + strlen(source);
    char *newData = (char *)malloc(newSize * sizeof(char));
    if (newData == NULL)
        return false;
    strcpy(newData, target->data);
    strcat(newData, source);
    free(target->data);
    target->data = newData;
    target->size = newSize;
    return true;
}

/**
 * @brief Get a simple string from the given String object
 *
 * @param str
 * @return char*
 */
char *toString(String *str) {
    return str != NULL && str->size - 1 > 0 && str->data != NULL ? str->data : "INVALID_STRING";
}

/**
 * @brief Replace a String object's data
 *
 * @param str
 * @param source
 */
void stringReplace(String *str, char *source) {
    free(str->data);
    str->size = strlen(source) + 1;
    str->data = (char *)malloc(str->size * sizeof(char));
    strcpy(str->data, source);
}

/**
 * @brief Free allocated memory of a String object
 *
 * @param string
 */
void stringFree(String *string) {
    free(string->data);
    free(string);
}

/**
 * @brief Clear the input buffer, because after every
 * screen option input, the enter (\\n) will be in the buffer
 * and ruining every user input
 *
 */
void clearInputBuffer() {
    while ((getchar()) != '\n')
        ;
}

/**
 * @brief Get the actual date
 *
 * @param date
 */
void getActualDate(String *date) {
    char *s = (char *)malloc(17 * sizeof(char));
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(s, "%d-%02d-%02d %02d:%02d",
            tm.tm_year + 1900, tm.tm_mon + 1,
            tm.tm_mday, tm.tm_hour, tm.tm_min);
    stringReplace(date, s);
    free(s);
}