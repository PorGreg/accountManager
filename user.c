#include "user.h"

#include <stdbool.h>
#include <stdlib.h>

#include "transaction.h"
#include "types.h"
#include "utils.h"

/**
 * @brief Insert a user to the linked list
 *
 * @param user linked list of users
 * @param id id of the user
 * @param username name of the user
 * @param password password of the user
 * @param account account balance of the user
 * @return User* - pointer for the linked list
 */
User *insertUser(User *user, int id, char *username, char *password, int account) {
    if (user == NULL) {
        user = (User *)malloc(sizeof(User));
        user->id = id;
        user->username = stringInit(username);
        user->password = stringInit(password);
        user->account = account;
        user->transactions = NULL;
        user->next = NULL;
    } else
        user->next = insertUser(user->next, id, username, password, account);
    return user;
}

/**
 * @brief Get the username for a user based on an ID
 *
 * @param users linked list of users
 * @param id which user you want
 * @return String* - pointer for the user's username
 */
User *getUser(User *users, int id) {
    for (User *user = users; user != NULL; user = user->next) {
        if (user->id == id)
            return user;
    }
    return NULL;
}

/**
 * @brief Check if the users is the same
 *
 * @param user1
 * @param user2
 * @return bool - will true if the given users are the same, otherwise false
 */
bool isSameUser(User *user1, User *user2) {
    return user1->id == user2->id;
}

/**
 * @brief Free allocated user
 *
 * @param transaction specified user
 */
void freeUser(User *user) {
    stringFree(user->username);
    stringFree(user->password);
    free(user);
}

/**
 * @brief Free allocated users
 *
 * @param transaction specified linked list of users
 */
void freeUsers(User *user) {
    if (user == NULL)
        return;
    if (user->next != NULL)
        freeUsers(user->next);
    freeUser(user);
}