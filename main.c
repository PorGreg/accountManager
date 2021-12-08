#include <stdio.h>
#include <stdlib.h>

#include "debugmalloc.h"
#include "filesystem.h"
#include "screens.h"
#include "types.h"
#include "user.h"
#include "utils.h"

/**
 * @brief Get the App State object based on data files
 *
 * @return AppState* - actual state of the application
 */
AppState *getAppState() {
    AppState *appState = (AppState *)malloc(sizeof(AppState));

    // set initial values
    appState->screen = S_WELCOME;
    appState->loggedUser = NULL;
    appState->errorMessage = stringInit(NULL);
    appState->message = stringInit(NULL);
    appState->date = stringInit(NULL);
    // if cant alloc variables in the beginning, exit from the application
    if (appState->errorMessage == NULL ||
        appState->message == NULL ||
        appState->date == NULL) {
        printf("Not enough memory! Exiting...");
        exit(1);
    }
    getActualDate(appState->date);
    appState->appEvent = EVENT_CONTINUE;
    appState->users = readUsers(appState);

    return appState;
}

/**
 * @brief Free allocated app data
 *
 * @param appState actual state of application
 */
void freeAppState(AppState *appState) {
    stringFree(appState->errorMessage);
    stringFree(appState->message);
    stringFree(appState->date);
    freeUsers(appState->users);
    free(appState);
}

int main() {
    // call to get the initial value
    AppState *appState = getAppState();
    // loop until the user wants to exit
    bool shouldExit = false;
    while (!shouldExit) {
        renderScreen(appState);
        // if EXIT event called, the app will quit
        if (appState->appEvent == EVENT_EXIT) {
            shouldExit = true;
            // if RESET event called, the app will reset itself with every data
        } else if (appState->appEvent == EVENT_RESET) {
            resetAppData(appState);
            freeAppState(appState);
            appState = getAppState();
            // otherwise it will continue
        } else {
            saveAppData(appState);
        }
    }
    freeAppState(appState);
    return 0;
}