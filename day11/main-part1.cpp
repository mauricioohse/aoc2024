#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#pragma GCC diagnostic ignored "-Wunused-parameter" 


#define INPUT "input.txt"
// #define INPUT "test.txt"
#define MAX_SIZE 100
char content[MAX_SIZE] = {0};
int content_count = 0;

void print_arr(char *arr, int arr_size)
{
    for (int i = 0; i < arr_size; i++)
        printf("%c", *(arr + i));
}

#define MAX_ARRAY 1000000
struct Array
{
    long long int items[MAX_ARRAY];
    int count;
} numbers;

int count_digits(long long int number)
{
    int count = 0;
    while(number!=0)
    {
        number = number/10;
        count++;
    }
    return count;
}

long long int left_half(long long int number)
{
    int count = count_digits(number);

    for(int i = 0; i < count/2; i++)
        number/=10;

    return number;
}

long long int right_half(long long int number)
{
    return number - left_half(number)*(pow(10, count_digits(number)/2));
}

int arr_insert(struct Array *arr, long long value, int pos)
{
    if (arr->count >= MAX_ARRAY) return -1; // or handle grow
    // shift [pos, count) one slot to the right
    size_t nmove = (size_t)(arr->count - pos);
    memmove(&arr->items[pos + 1], &arr->items[pos], nmove * sizeof(arr->items[0]));
    arr->items[pos] = value;
    arr->count++;
    return 0;
}


int main()
{
    memset(&numbers, 0, sizeof(numbers));

    freopen(INPUT, "r", stdin);

    { // Read character by character until the end of the file
        int ch;
        while ((ch = getchar()) != EOF)
        {
            // Only add characters that are part of the map, ignoring newlines
            if (ch != '\0')
            {
                content[content_count++] = ch;
            }
        }
    }

    print_arr(content, content_count);
    printf("\n");

    // transform char array into int array
    char *str;
    str = strtok(content, " ");
    while (str != NULL)
    {
        numbers.items[numbers.count++] = strtoll(str, nullptr, 10);
        str = strtok(nullptr, " ");
    }

    for (int i = 0; i < numbers.count; i++)
        printf("%lld ", numbers.items[i]);

    // printf("\ntesting appending: ");
    // arr_insert(&numbers, 10, 1);
    
    for (int i = 0; i < numbers.count; i++)
        printf("%lld ", numbers.items[i]);

    printf("\n");

    for(int it =0; it < 25; it++)
    {
        // main interation loop
        for (long long int i = 0; i < numbers.count; i++)
        {
            // for each member...
            if (numbers.items[i] == 0)
            {
                numbers.items[i] = 1;
                continue;
            }
            else if(count_digits(numbers.items[i])%2 == 0)
            {
                long long int number = numbers.items[i];
                numbers.items[i] = right_half(number);
                arr_insert(&numbers, left_half(number),i);
                i++;
            }
            else
            {
                numbers.items[i]=numbers.items[i]*2024ll;
            }
        }
        
        printf("iteration %d, count %d\n ", it, numbers.count);
        // print numbers
        // for (int i = 0; i < numbers.count; i++)
        //     printf("%lld ", numbers.items[i]);
        // printf("\n");

    }

    printf("part1: %d", numbers.count);
    return 0;
}