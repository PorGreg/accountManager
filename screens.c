#include "screens.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "filesystem.h"
#include "transaction.h"
#include "types.h"
#include "user.h"
#include "utils.h"

/**
 * @brief Clear the terminal
 * Used command based on the current platform
 *
 */
void clrscr() {
#ifdef _WIN32
    /* windows */
    system("cls")
#else
    /* posix */
    system("clear");
#endif
}

// --------------
// SCREEN METHODS
// --------------

// welcome

void renderWelcome(AppState *appState) {
    printf(
        "| Welcome here\n\n"
        "1. Sign in\n"
        "2. Sign up\n"
        "3. Exit\n"
        "4. Reset application\n");
    pickScreenOption(appState);
}

void pickWelcomeOption(AppState *appState, int option) {
    switch (option) {
        case 1:
            appState->screen = S_LOGIN;
            break;
        case 2:
            appState->screen = S_REGISTER;
            break;
        case 3:
            appState->appEvent = EVENT_EXIT;
            break;
        case 4:
            appState->appEvent = EVENT_RESET;
            break;
        default:
            if (option > 0)
                printf("Choose an option that is in the screen!\n");
            return;
    }
}

// login

void renderLogin(AppState *appState) {
    printf("| Sign in\n\n");
    loginUser(appState);
}

void loginUser(AppState *appState) {
    // get username and password from the terminal
    printf("Username: ");
    String *username = getStringFromUser();
    printf("Password: ");
    String *password = getStringFromUser();

    // compare it will all of the users in the application
    User *currentUser = appState->users;
    while (currentUser != NULL && appState->loggedUser == NULL) {
        if (stringCompare(username, currentUser->username) &&
            stringCompare(password, currentUser->password))
            appState->loggedUser = currentUser;
        currentUser = currentUser->next;
    }

    // if no user found, write an error message
    if (appState->loggedUser == NULL) {
        stringCopy(appState->errorMessage, "\nWrong username of password!");
        appState->screen = S_WELCOME;
        // otherwise return to home page
    } else {
        readTransactions(appState);
        appState->screen = S_HOME;
    }

    stringFree(username);
    stringFree(password);
}

// register

void renderRegister(AppState *appState) {
    printf("| Sign up\n\n");
    registerUser(appState);
}

void registerUser(AppState *appState) {
    // get username and password from the terminal
    printf("Username: ");
    String *username = getStringFromUser();
    printf("Password: ");
    String *password = getStringFromUser();

    // compare it will all of the users in the application
    User *currentUser = appState->users;
    bool userFound = false;
    // get maxId (need for the ID)
    int maxId = currentUser != NULL ? currentUser->id : 0;
    while (currentUser != NULL && !userFound) {
        if (stringCompare(username, currentUser->username))
            userFound = true;
        if (currentUser->id > maxId)
            maxId = currentUser->id;
        currentUser = currentUser->next;
    }

    // if user was found with the same username, write an error
    if (userFound) {
        stringCopy(appState->errorMessage, "\nThis username already been used!");
        // otherwise create a new user
    } else {
        User *user = insertUser(appState->users, maxId + 1, username->data, password->data, 5000);
        if (appState->users == NULL)
            appState->users = user;
        stringCopy(appState->message, "\nSuccessfully registered to our system!");
    }

    appState->screen = S_WELCOME;
    stringFree(username);
    stringFree(password);
}

// home

void renderHome(AppState *appState) {
    printf(
        "| Home\n\n"
        "1. Account history\n"
        "2. New transaction\n"
        "3. Account settings\n"
        "4. Log out\n");
    pickScreenOption(appState);
}

void pickHomeOption(AppState *appState, int option) {
    switch (option) {
        case 1:
            appState->screen = S_ACCOUNT_HISTORY;
            break;
        case 2:
            appState->screen = S_NEW_TRANSACTION;
            break;
        case 3:
            appState->screen = S_ACCOUNT_SETTINGS;
            break;
        case 4:
            freeTransactions(appState->loggedUser->transactions);
            appState->loggedUser = NULL;
            appState->screen = S_WELCOME;
            break;
        default:
            if (option > 0)
                printf("Choose an option that is in the screen!\n");
            pickScreenOption(appState);
            break;
    }
}

// account history

void renderAccountHistory(AppState *appState) {
    printf(
        "| Account history\n\n"
        "%8s %8s %12s %20s\n",
        "To", "From", "Amount", "Date");
    for (Transaction *transaction = appState->loggedUser->transactions;
         transaction != NULL; transaction = transaction->next)
        printf("%8s %8s %11d$ %20s\n",
               toString(transaction->usernameTo),
               toString(transaction->usernameFrom),
               transaction->amount,
               toString(transaction->date));
    printf("\nPress enter to exit to home page.");
    getchar();
    appState->screen = S_HOME;
}

// new transaction

void renderNewTransaction(AppState *appState) {
    if (appState->loggedUser->account == 0) {
        stringCopy(appState->errorMessage, "\nDo not have any money!");
        appState->screen = S_HOME;
        return;
    }
    printf(
        "| New transaction\n\n"
        "List of users:\n\n"
        "%8s %10s\n",
        "User ID", "Username");
    // print all user in the application
    for (User *user = appState->users; user != NULL; user = user->next)
        if (user->id != appState->loggedUser->id)
            printf("%8d %10s\n", user->id, toString(user->username));

    // get the requested user from the terminal
    printf("\nEnter the user id you want to sent money: ");
    int id;
    User *userTo = NULL;
    while (userTo == NULL) {
        scanf("%d", &id);
        if (id == appState->loggedUser->id) {
            printf("This is your own id! ");
            continue;
        }
        userTo = getUser(appState->users, id);
        if (userTo == NULL)
            printf("This user does not exists! ");
    }

    // get the requested ammount from the terminal
    printf("\n\nHow many $ do you want to send? (You have: %d$) ", appState->loggedUser->account);
    int amount;
    bool isGivenAvailable = false;
    while (!isGivenAvailable) {
        scanf("%d", &amount);
        if (amount > appState->loggedUser->account)
            printf("You do not have enough money for this! ");
        else if (amount < 0)
            printf("You cannot sent negative amount! ");
        else
            isGivenAvailable = true;
    }

    // get the actual date for the transaction
    getActualDate(appState->date);
    // insert it to the linked list of transactions
    appState->loggedUser->transactions =
        insertTransaction(appState->loggedUser->transactions, appState->users,
                          id, appState->loggedUser->id, amount, toString(appState->date));
    appState->loggedUser->account -= amount;
    getUser(appState->users, id)->account += amount;
    appState->screen = S_HOME;
    clearInputBuffer();
}

// account settings

void renderAccountSettings(AppState *appState) {
    printf(
        "| Account settings\n\n"
        "1. Change username\n"
        "2. Change password\n");
    pickScreenOption(appState);
}

void pickAccountSettingsOption(AppState *appState, int option) {
    switch (option) {
        case 1:
            appState->screen = S_CHANGE_USERNAME;
            break;
        case 2:
            appState->screen = S_CHANGE_PASSWORD;
            break;
        default:
            if (option > 0)
                printf("Choose an option that is in the screen!\n");
            pickScreenOption(appState);
            break;
    }
}

// change username

void renderChangeUsername(AppState *appState) {
    printf("| Change username\n\n");
    changeUsername(appState);
}

void changeUsername(AppState *appState) {
    printf("New username: ");
    // get new username from the user
    String *newUsername = getStringFromUser();
    // search for the given user
    for (User *user = appState->users; user != NULL; user = user->next) {
        // if found, edit its username
        if (isSameUser(user, appState->loggedUser)) {
            stringReplace(user->username, toString(newUsername));
        }
    }
    appState->screen = S_HOME;
    stringFree(newUsername);
}

// change password

void renderChangePassword(AppState *appState) {
    printf("| Change username\n\n");
    changePassword(appState);
}

void changePassword(AppState *appState) {
    printf("New username: ");
    // get new password from the user
    String *newPassword = getStringFromUser();
    // search for the given user
    for (User *user = appState->users; user != NULL; user = user->next) {
        // if found, edit its password
        if (isSameUser(user, appState->loggedUser)) {
            stringReplace(user->password, toString(newPassword));
        }
    }
    appState->screen = S_HOME;
    stringFree(newPassword);
}

// main rendering methods

void renderHeader(AppState *appState) {
    printf(
        "---------------------------------------\n"
        "| BANK ACCOUNT MANAGER - by Pór Gergő |\n"
        "---------------------------------------\n"
        "Time: %s\n",
        toString(appState->date));
    if (appState->loggedUser != NULL)
        printf(
            "User: %s\n"
            "Account balance: %d$",
            appState->loggedUser->username->data,
            appState->loggedUser->account);
    if (appState->errorMessage->size - 1 > 0)
        printf(
            "\nERRORS: %s\n",
            appState->errorMessage->data);
    if (appState->message->size - 1 > 0)
        printf("\nMESSAGE: %s\n",
               appState->message->data);
    printf("\n\n");
}

void renderScreen(AppState *appState) {
    clrscr();
    getActualDate(appState->date);
    renderHeader(appState);
    switch (appState->screen) {
        case S_WELCOME:
            renderWelcome(appState);
            break;
        case S_LOGIN:
            renderLogin(appState);
            break;
        case S_REGISTER:
            renderRegister(appState);
            break;
        case S_HOME:
            renderHome(appState);
            break;
        case S_ACCOUNT_HISTORY:
            renderAccountHistory(appState);
            break;
        case S_NEW_TRANSACTION:
            renderNewTransaction(appState);
            break;
        case S_ACCOUNT_SETTINGS:
            renderAccountSettings(appState);
            break;
        case S_CHANGE_USERNAME:
            renderChangeUsername(appState);
            break;
        case S_CHANGE_PASSWORD:
            renderChangePassword(appState);
            break;
        default:
            renderWelcome(appState);
            break;
    }
}

void pickScreenOption(AppState *appState) {
    stringFree(appState->errorMessage);
    stringFree(appState->message);
    appState->errorMessage = stringInit(NULL);
    appState->message = stringInit(NULL);
    int option;
    option = getchar() - '0';
    switch (appState->screen) {
        case S_WELCOME:
            pickWelcomeOption(appState, option);
            break;
        case S_HOME:
            pickHomeOption(appState, option);
            break;
        case S_ACCOUNT_SETTINGS:
            pickAccountSettingsOption(appState, option);
            break;
        case S_LOGIN:
        case S_REGISTER:
            break;
        default:
            return;
    }
    clearInputBuffer();
}