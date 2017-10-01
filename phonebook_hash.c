
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "phonebook_hash.h"

/* original version */
entry *findName(char lastName[], entry *pHead[])
{
    unsigned int hashVal = BKDRHash(lastName);
    while (pHead[hashVal] != NULL) {
        if (strcasecmp(lastName, pHead[hashVal]->lastName) == 0)
            return pHead[hashVal];
        pHead[hashVal] = pHead[hashVal]->pNext;
    }
    return NULL;
}

void append(char lastName[], entry *e[], entry *pHead[])
{
    unsigned int hashVal = BKDRHash(lastName);

    /* allocate memory for the new entry and put lastName */
    entry *newEntry = (entry *)malloc(sizeof(entry));
    strcpy(newEntry->lastName, lastName);
    newEntry->pNext = NULL;

    if(e[hashVal] == NULL) {
        pHead[hashVal] = e[hashVal] = newEntry;
        return;
    }
    e[hashVal]->pNext = newEntry;
    e[hashVal] = newEntry;
}
// BKDR Hash Function
unsigned int BKDRHash(char *str)
{
    unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
    unsigned int hash = 0;
    while (*str) {
        hash = hash * seed + (*str++);
    }
    return (hash & 0x7FFFFFFF) % HASH_TABLE_SIZE;
}
