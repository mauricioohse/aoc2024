#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void print_arr(int *arr, size_t size)
{
    printf("printing arr\n");
    for (size_t i=0; i<size; i++)
        {
            printf("%d ",arr[i]);
        }
    printf("\n");
}

int compare_int(const void * first,const void * second)
{
    if (*(int*)first > *(int*)second) return 1;
    if (*(int*)first <*(int*) second) return -1;
    return 0;
}

int main(){

    FILE* f = fopen("input.txt", "r");

    int col0[1000] = {0};
    int col1[1000] = {0};
    size_t i = 0;

    char line[100] = {0};
    while (fgets(line,100,f))
    {
        char * tok = strtok(line," ");
        col0[i] = atoi(tok);
        tok = strtok(NULL," ");
        col1[i] = atoi(tok);
        i++;
    }

    qsort(col0, i, sizeof(int),compare_int);
    qsort(col1, i, sizeof(int),compare_int);

    size_t diff = 0;

    for (size_t idx = 0; idx < i; idx++)
    {
        diff+= labs(col0[idx]-col1[idx]);
    }

    printf("result part 1: %lld\n",diff);

    // part 2
    size_t similarity = 0;
    for (size_t k = 0; k < i; k++)
    {
        int search_value = col0[k];
        int repeat_count = 0;
        for (size_t j = 0; j < i; j++)
        {
            if (col1[j] == search_value)
            {
                repeat_count++;
            }
        }
        similarity += repeat_count * search_value;
    }

    printf("result part 2: %lld\n", similarity);

    return 0;
}