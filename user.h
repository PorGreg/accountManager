#ifndef USER
#define USER

#include "types.h"

User *insertUser(User *user, int id, char *username, char *password, int account);
User *getUser(User *users, int id);
bool isSameUser(User *user1, User *user2);
void freeUser(User *user);
void freeUsers(User *user);

#endif