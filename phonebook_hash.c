
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "phonebook_hash.h"
#define ELF
/* original version */
entry *findName(char lastName[], entry *pHead[])
{
    unsigned int hashVal;
#if defined BKDR
    hashVal = BKDRHash(lastName);
#endif

#if defined ELF
    hashVal = ELFHash(lastName);
#endif

    while (pHead[hashVal] != NULL) {
        if (strcasecmp(lastName, pHead[hashVal]->lastName) == 0)
            return pHead[hashVal];
        pHead[hashVal] = pHead[hashVal]->pNext;
    }
    return NULL;
}

void append(char lastName[], entry *e[], entry *pHead[])
{
    unsigned int hashVal;
#if defined BKDR
    hashVal = BKDRHash(lastName);
#endif

#if defined ELF
    hashVal = ELFHash(lastName);
#endif
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

unsigned int ELFHash(char *str)
{
    unsigned int h = 0, g;

    while(*str) {
        h = (h << 4) + (*str++);
        g = h & 0xf0000000L;
        if(g) {
            h ^= g >> 24;
        }

        h &= ~g;
    }

    return h % HASH_TABLE_SIZE;
}
