#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ammenu.h"

#if defined(_WIN32) || defined(_WIN64)  // windows

#include <windows.h>

/**
 * Print the given text to the terminal in the middle of the screen
 * 
 * */
void amprint(char* text) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;

    printf("%*c%s\n", (int)(columns / 2 - strlen(text)), ' ', text);
}

#else  // unix

#include <sys/ioctl.h>
#include <unistd.h>

/**
 * Print the given text to the terminal in the middle of the screen
 * 
 * */
void amprint(char* text) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    printf("%*c%s\n", (int)(w.ws_col / 2 - strlen(text)), ' ', text);
}

#endif

void render_screen() {
    system("clear");
    printf("%*c", 5, '\n');
    amprint("Account Managment System\n");
    render_menu()
}

void render_menu() {
}