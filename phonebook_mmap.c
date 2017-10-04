#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>

#include "phonebook_mmap.h"

#define BUFFER_SIZE 1024

/* TODO: FILL YOUR OWN IMPLEMENTATION HERE! */
entry *findName(char *lastName, entry *pHead[])
{
    unsigned int hashVal = BKDRHash(lastName);
    while (pHead[hashVal] != NULL) {
        if (strcasecmp(lastName, pHead[hashVal]->lastName) == 0)
            return pHead[hashVal];
        pHead[hashVal] = pHead[hashVal]->pNext;
    }
    return NULL;
}

void append(void *addr, entry *pHead[], entry *tail[])
{
    entry *e = (entry*) addr ;
    e->pNext = NULL;

    unsigned int hashVal = BKDRHash(e->lastName);
    if(pHead[hashVal] == NULL) {
        pHead[hashVal] = tail[hashVal] = e;
        return;
    }

    tail[hashVal]->pNext = e;
    tail[hashVal] = e;

}

unsigned int BKDRHash(char *str)
{
    unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
    unsigned int hash = 0;
    while (*str) {
        hash = hash * seed + (*str++);
    }
    return (hash & 0x7FFFFFFF) % HASH_TABLE_SIZE;
}

void format_data(char *mod, char *org)
{
    FILE *input_fd = fopen(org, "r");
    FILE *output_fd = fopen(mod,"wb+");

    int length;
    entry e;
    char line[BUFFER_SIZE];
    while (fgets(line, sizeof(line), input_fd)) {
        memset(e.lastName, '\0', MAX_LAST_NAME_SIZE);
        strncpy(e.lastName, line, MAX_LAST_NAME_SIZE);
        length = strlen(e.lastName);
        e.lastName[length - 1] = '\0';
        fwrite(&e, sizeof(entry), 1, output_fd);
    }

    fclose(input_fd);
    fclose(output_fd);
}

off_t file_size(char *path)
{
    struct stat st;
    stat(path, &st);
    return st.st_size;
}