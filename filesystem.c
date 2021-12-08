#include "filesystem.h"

#include <stdlib.h>
#include <string.h>

#include "transaction.h"
#include "types.h"
#include "user.h"
#include "utils.h"

/**
 * @brief Read one line from a file (maximum 128 character) and then return as an allocated string
 *
 * @param file Input file
 * @return char* - allocated string containing the line
 */
char *readLine(FILE *file) {
    char *line = (char *)malloc(128 * sizeof(char));
    if (!fgets(line, 128, file)) {
        free(line);
        return NULL;
    }

    while (line[strlen(line) - 1] != '\n') {
        line = (char *)realloc(line, (strlen(line) + 129) * sizeof(char));
        char tmp[129];
        fgets(tmp, 129, file);
        strcat(line, tmp);
    }

    line[strlen(line) - 1] = '\0';
    return line;
}

/**
 * @brief Read the user data from a specified file
 *
 * @param appState the state of the application
 * @return User* - a pointer for the first User (we can call it as a head)
 */
User *readUsers(AppState *appState) {
    FILE *file = openFile(appState, "users.csv", "rt");

    char *line;

    User *head = NULL;

    // read until we reach the end of the file
    while ((line = readLine(file)) != NULL) {
        // tmp variable for str -> int conversion
        char *linePart;

        // get id
        int id;
        linePart = strtok(line, ";");
        sscanf(linePart, "%d", &id);

        // get username
        char *username = strtok(NULL, ";");

        // get password
        char *password = strtok(NULL, ";");

        // get account
        int account;
        linePart = strtok(NULL, ";");
        sscanf(linePart, "%d", &account);

        // insert it as a user
        head = insertUser(head, id, username, password, account);

        // free memory
        free(line);
    }

    fclose(file);
    return head;
}

/**
 * @brief Read transactions from a specified file.
 *
 * @param appState the state of the application
 */
void readTransactions(AppState *appState) {
    FILE *file = openFile(appState, "transactions.csv", "rt");

    char *line;

    Transaction *transaction = NULL;

    // read until we reach the end of the file
    while ((line = readLine(file)) != NULL) {
        // tmp variable for str -> int conversion
        char *linePart;

        // get idTo
        int idTo;
        linePart = strtok(line, ";");
        sscanf(linePart, "%d", &idTo);

        // get idFrom
        int idFrom;
        linePart = strtok(NULL, ";");
        sscanf(linePart, "%d", &idFrom);

        // get amount
        int amount;
        linePart = strtok(NULL, ";");
        sscanf(linePart, "%d", &amount);

        // get date
        char *date = strtok(NULL, ";");

        if (appState->loggedUser->id == idTo || appState->loggedUser->id == idFrom) {
            appState->loggedUser->transactions =
                insertTransaction(
                    appState->loggedUser->transactions,
                    appState->users, idTo, idFrom, amount, date);
        }

        // free memory
        free(line);
    }
    fclose(file);
}

/**
 * @brief Open a file and return its pointer
 *
 * @param appState actual state of the app
 * @param fileName name of the file
 * @param mode mode for the file
 * @return FILE* - pointer for the file
 */
FILE *openFile(AppState *appState, char *fileName, char *mode) {
    FILE *file = fopen(fileName, mode);
    if (file == NULL) {
        stringCopy(appState->errorMessage, "\n- Cannot open file with name: ");
        stringCopy(appState->errorMessage, fileName);
        return NULL;
    }
    return file;
}

/**
 * @brief Save the app data in specified .csv files
 *
 * @param appState actual state of the app
 */
void saveAppData(AppState *appState) {
    FILE *userFile = openFile(appState, "users.csv", "wt");
    if (userFile != NULL) {
        for (User *user = appState->users; user != NULL; user = user->next)
            fprintf(userFile, "%d;%s;%s;%d\n",
                    user->id, user->username->data, user->password->data, user->account);

        fclose(userFile);
    }
    if (appState->loggedUser != NULL) {
        FILE *transactionFile = openFile(appState, "transactions.csv", "wt");
        if (transactionFile != NULL) {
            for (Transaction *transaction = appState->loggedUser->transactions;
                 transaction != NULL; transaction = transaction->next)
                fprintf(transactionFile, "%d;%d;%d;%s\n",
                        transaction->idTo, transaction->idFrom,
                        transaction->amount, toString(transaction->date));
            fclose(transactionFile);
        }
    }
}

/**
 * @brief Reset the application data in the files
 *
 * @param appState actual state of the app
 */
void resetAppData(AppState *appState) {
    FILE *userFile = openFile(appState, "users.csv", "wt");
    FILE *transactionFile = openFile(appState, "transactions.csv", "wt");
    fprintf(userFile, "%d;%s;%s;%d\n", 1, "admin", "admin", 5000);
    fclose(userFile);
    fclose(transactionFile);
}