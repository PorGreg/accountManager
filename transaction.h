#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "types.h"

Transaction *insertTransaction(
    Transaction *transaction, User *users, int idTo, int idFrom, int amount, char *date);
void freeTransaction(Transaction *transaction);
void freeTransactions(Transaction *transaction);

#endif