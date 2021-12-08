#ifndef SCREENS_H
#define SCREENS_H

#include "types.h"

void renderScreen(AppState *appState);
void renderHeader(AppState *appState);
void renderWelcome(AppState *appState);
void renderLogin(AppState *appState);
void renderRegister(AppState *appState);
void renderHome(AppState *appState);
void renderAccountHistory(AppState *appState);
void renderNewTransaction(AppState *appState);
void renderAccountSettings(AppState *appState);
void renderChangeUsername(AppState *appState);
void renderChangePassword(AppState *appState);
void loginUser(AppState *appState);
void registerUser(AppState *appState);
void changeUsername(AppState *appState);
void changePassword(AppState *appState);
void pickScreenOption(AppState *appState);
void pickWelcomeOption(AppState *appState, int option);
void pickHomeOption(AppState *appState, int option);
void pickAccountSettingsOption(AppState *appState, int option);

#endif