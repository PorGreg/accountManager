#include "transaction.h"

#include <stdlib.h>

#include "types.h"
#include "user.h"
#include "utils.h"

/**
 * @brief Insert a transaction in the given linked list
 *
 * @param transaction a linked list for the saving
 * @param users a list of users for usernames
 * @param idTo id of where the money went
 * @param idFrom id of where the money from
 * @param amount how much money was sent
 * @param date when the transaction was sent
 * @return Transaction* - pointer for the head of the list
 */
Transaction *insertTransaction(
    Transaction *transaction, User *users, int idTo, int idFrom, int amount, char *date) {
    if (transaction == NULL) {
        transaction = (Transaction *)malloc(sizeof(Transaction));
        transaction->idTo = idTo;
        transaction->idFrom = idFrom;
        transaction->amount = amount;
        transaction->date = stringInit(date);
        transaction->usernameTo = getUser(users, transaction->idTo)->username;
        transaction->usernameFrom = getUser(users, transaction->idFrom)->username;
        transaction->next = NULL;
    } else
        transaction->next = insertTransaction(transaction->next, users, idTo, idFrom, amount, date);
    return transaction;
}

/**
 * @brief Free allocated transaction
 *
 * @param transaction specified transaction
 */
void freeTransaction(Transaction *transaction) {
    stringFree(transaction->date);
    free(transaction);
}

/**
 * @brief Free allocated transactions
 *
 * @param transaction specified linked list of transactions
 */
void freeTransactions(Transaction *transaction) {
    if (transaction == NULL)
        return;
    if (transaction->next != NULL)
        freeTransactions(transaction->next);
    freeTransaction(transaction);
}