#ifndef _PHONEBOOK_H
#define _PHONEBOOK_H

#define MAX_LAST_NAME_SIZE 16
#define HASH_TABLE_SIZE 1003

#include <sys/stat.h>
/* TODO: After modifying the original version, uncomment the following
 * line to set OPT properly */
typedef struct __PHONE_BOOK_ENTRY {
    char lastName[MAX_LAST_NAME_SIZE];
    struct __PHONE_BOOK_ENTRY *pNext;
    struct _PERSONAL_INFO *detail;
} entry;

typedef struct _PERSONAL_INFO {
    char firstName[16];
    char email[16];
    char phone[10];
    char cell[10];
    char addr1[16];
    char addr2[16];
    char city[16];
    char state[2];
    char zip[5];
} _PERSONAL_INFO;

entry *findName(char *lastName, entry *pHead[]);
void append(void *addr, entry *pHead[], entry *tail[]);
unsigned int BKDRHash(char *str);
void format_data(char *org, char *mod);
off_t file_size(char *path);
#endif
