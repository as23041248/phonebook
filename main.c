#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include IMPL

#define DICT_FILE "./dictionary/words.txt"
#if defined (OPT)
#define OUT_FILE "opt.txt"
#endif

#if defined (HASH)
#define OUT_FILE "hash.txt"
#endif

#if defined (ORIG)
#define OUT_FILE "orig.txt"
#endif

#if defined (HASH_MMAP)
#define DICT_FILE_BIN DICT_FILE ".bin"
#define OUT_FILE "hash_mmap.txt"
#endif

static double diff_in_second(struct timespec t1, struct timespec t2)
{
    struct timespec diff;
    if (t2.tv_nsec-t1.tv_nsec < 0) {
        diff.tv_sec  = t2.tv_sec - t1.tv_sec - 1;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec + 1000000000;
    } else {
        diff.tv_sec  = t2.tv_sec - t1.tv_sec;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec;
    }
    return (diff.tv_sec + diff.tv_nsec / 1000000000.0);
}

int main(int argc, char *argv[])
{
    int i = 0;
    struct timespec start, end;
    double cpu_time1, cpu_time2;


#if defined (HASH) || defined (ORIG) || defined (OPT)
    /* check file opening */
    FILE *fp;
    fp = fopen(DICT_FILE, "r");
    char line[MAX_LAST_NAME_SIZE];
    if (fp == NULL) {
        printf("cannot open the file\n");
        return -1;
    }

    /* build the entry */
#if defined (HASH)
    entry *pHead[HASH_TABLE_SIZE], *e[HASH_TABLE_SIZE];
    for (i = 0; i < HASH_TABLE_SIZE; i++) {
        pHead[i] = e[i] = NULL;
    }
#endif

#if defined (ORIG) || defined (OPT)
    entry *pHead, *e;
    pHead = e = NULL;

#endif

#endif

#if defined (HASH_MMAP)
    entry *pHead[HASH_TABLE_SIZE], *e[HASH_TABLE_SIZE];
    format_data(DICT_FILE_BIN, DICT_FILE);
    for (i = 0; i < HASH_TABLE_SIZE; i++) {
        pHead[i] = e[i] = NULL;
    }
#endif

#if defined(__GNUC__)
    __builtin___clear_cache((char *)pHead, (char *)pHead + sizeof(entry));
#endif
    clock_gettime(CLOCK_REALTIME, &start);

#if defined(HASH) || defined(ORIG) || defined(OPT)
    while (fgets(line, sizeof(line), fp)) {
        while (line[i] != '\0')
            i++;
        line[i - 1] = '\0';
        i = 0;

#if defined (HASH)
        append(line, e, pHead);
#endif

#if defined(ORIG) || defined(OPT)
        if(!pHead) {
            pHead = e = append(line, e);
        } else {
            e = append(line, e);
        }
#endif

    }
#endif

#if defined(HASH_MMAP)
    int fd = open(DICT_FILE_BIN, O_RDONLY);
    int size_of_file = file_size(DICT_FILE_BIN);
    int size_of_entry = sizeof(entry);
    void *block_start = mmap(NULL, size_of_file, PROT_WRITE, MAP_PRIVATE, fd, 0);
    void *iterate = block_start;

    while(iterate < block_start + size_of_file) {
        append(iterate, pHead, e);
        iterate += size_of_entry;
    }
    close(fd);

#endif

    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time1 = diff_in_second(start, end);

    /* close file as soon as possible */
#if defined (HASH) || defined (ORIG) || defined(OPT)
    fclose(fp);
#endif

#if defined(HASH) || defined(HASH_MMAP)
    for (i = 0; i < HASH_TABLE_SIZE; i++) {
        e[i] = pHead[i];
    }
#endif

#if defined(ORIG) || defined(OPT)
    e = pHead;
#endif

    /* the givn last name to find */
    char input[MAX_LAST_NAME_SIZE] = "zyxel";

    /* reset the pointer location */
#if defined(HASH) || defined(HASH_MMAP)
    for (i = 0; i < HASH_TABLE_SIZE; i++) {
        e[i] = pHead[i];
    }
#endif

#if defined(ORIG) || defined(OPT)
    e = pHead;
#endif

#if defined(ORIG) || defined(OPT) || defined(HASH) || defined(HASH_MMAP)
    assert(findName(input, e) &&
           "Did you implement findName() in " IMPL "?");
    assert(0 == strcmp(findName(input, e)->lastName, "zyxel"));
#endif


    /* reset the pointer location */
#if defined(HASH) || defined(HASH_MMAP)
    for (i = 0; i < HASH_TABLE_SIZE; i++) {
        e[i] = pHead[i];
    }
#endif

#if defined(ORIG) || defined(OPT)
    e = pHead;
#endif

#if defined(__GNUC__)
    __builtin___clear_cache((char *) pHead, (char *) pHead + sizeof(entry));
#endif
    /* compute the execution time */
    clock_gettime(CLOCK_REALTIME, &start);
    findName(input, e);
    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time2 = diff_in_second(start, end);

    FILE *output = fopen(OUT_FILE, "a");
    fprintf(output, "append() findName() %lf %lf\n", cpu_time1, cpu_time2);
    fclose(output);

    // printf("execution time of append() : %lf sec\n", cpu_time1);
    // printf("execution time of findName() : %lf sec\n", cpu_time2);

#if defined(HASH)

    entry *temp = NULL;
    for (i = 0; i < HASH_TABLE_SIZE; i++) {
        temp = pHead[i];
        while(temp) {
            pHead[i] = pHead[i]->pNext;
            free(temp);
            temp = pHead[i];
        }
    }
#endif

#if defined(ORIG) || defined(OPT)
    entry *temp = NULL;
    temp = pHead;
    while(temp) {
        pHead = pHead->pNext;
        free(temp);
        temp = pHead;
    }
#endif

#if defined(HASH_MMAP)
    munmap(block_start, size_of_file);
#endif

    return 0;
}
