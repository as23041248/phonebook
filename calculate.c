#include <stdio.h>
#include <stdlib.h>
# define TEST_CASE_NUM 4
typedef struct {
    char output[20];
    double sum_a;
    double sum_f;
} testcase;
int main(void)
{
    int i = 0,j = 0;
    char append[50], find[50];
    FILE *fp, *output;

    testcase cases[TEST_CASE_NUM] = {
        {"orig.txt"},
        {"opt.txt"},
        {"hash.txt"},
        {"hash_mmap.txt"}
    };


    for (i = 0; i < TEST_CASE_NUM; i++) {
        fp = fopen(cases[i].output, "r");
        if (!fp) {
            printf("ERROR opening input file orig.txt\n");
            exit(0);
        }
        double append_time = 0.0, find_time = 0.0;
        for (j = 0; j < 100; j++) {
            if (feof(fp)) {
                printf("ERROR: You need 100 datum instead of %d\n", j);
                printf("run 'make run' longer to get enough information\n\n");
                exit(0);
            }
            fscanf(fp, "%s %s %lf %lf\n", append, find, &append_time, &find_time);
            cases[i].sum_a += append_time;
            cases[i].sum_f += find_time;
        }
        fclose(fp);
    }

    /* output */
    output = fopen("output.txt", "w");

    fprintf(output, "append()");
    for (i = 0; i < TEST_CASE_NUM; i++) {
        fprintf(output, " %lf", cases[i].sum_a / 100.0);
    }

    fprintf(output, "\nfindName()");
    for (i = 0; i < TEST_CASE_NUM; i++) {
        fprintf(output, " %lf", cases[i].sum_f / 100.0);
    }

    fclose(output);

    return 0;
}
