#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "phonebook_orig.h"

/* original version */
entry *findName(char lastName[], entry *pHead)
{
    while (pHead != NULL) {
        if (strcasecmp(lastName, pHead->lastName) == 0)
            return pHead;
        pHead = pHead->pNext;
    }
    return NULL;
}

entry *append(char lastName[], entry *e)
{
    /* allocate memory for the new entry and put lastName */
    entry *newEntry = (entry *)malloc(sizeof(entry));
    strcpy(newEntry->lastName, lastName);
    newEntry->pNext = NULL;

    if (e == NULL) {
        e = newEntry;
        return e;
    }

    e->pNext = newEntry;
    e = e->pNext;

    return e;
}
