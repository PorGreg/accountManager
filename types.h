#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>

enum Screen { S_WELCOME,
              S_LOGIN,
              S_REGISTER,
              S_HOME,
              S_ACCOUNT_HISTORY,
              S_NEW_TRANSACTION,
              S_ACCOUNT_SETTINGS,
              S_CHANGE_USERNAME,
              S_CHANGE_PASSWORD };

enum AppEvent {
    EVENT_CONTINUE,
    EVENT_EXIT,
    EVENT_RESET
};

typedef struct String {
    int size;
    char *data;
} String;

typedef struct Transaction {
    int idTo, idFrom, amount;
    String *date, *usernameTo, *usernameFrom;
    struct Transaction *next;
} Transaction;

typedef struct User {
    int id, account;
    String *username, *password;
    struct User *next;
    struct Transaction *transactions;
} User;

typedef struct AppState {
    struct User *loggedUser;
    struct User *users;
    enum Screen screen;
    enum AppEvent appEvent;
    String *errorMessage, *message, *date;
} AppState;

#endif