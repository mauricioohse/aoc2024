#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#pragma GCC diagnostic ignored "-Wunused-parameter" 

#define TODO(text) {printf(text); exit(-1);}

#define INPUT "input.txt"
// #define INPUT "test.txt"
#define MAX_SIZE 100
char content[MAX_SIZE] = {0};
int content_count = 0;

struct HashEntry
{
    long long int key;
    long long int hash;
    long long int value;
    HashEntry *next;
};

#define HASH_MAP_SIZE (1<<20)

struct HashMap
{
    HashEntry entries[HASH_MAP_SIZE+2];
};

HashMap *MAP;

long long int hashkey(long long int key)
{
    return (key%HASH_MAP_SIZE +1);
}

long long int hash_append(long long int key, long long int value, HashMap * map)
{
    // return 0 on success, 1 in case entry was already found
    long long int hash = hashkey(key);

    HashEntry *entry = &map->entries[hash];
    // assume hashmap was initialized with nullptr in last hash_entry
    while(entry->next!=nullptr)
    {
        if(entry->key == key) return 1; // we already had the entry, just return
        entry = entry->next;
    }

    if (entry->next== nullptr)
    {
        // we didnt find the entry already, so now we append before the last entry
        HashEntry *new_entry = (HashEntry *)malloc(sizeof(HashEntry));
        memset(new_entry,0, sizeof(HashEntry));

        entry->key=key;
        entry->value=value;
        entry->next = new_entry;
        printf("appended key %lld value %lld at hash %lld\n", key, value, hash);
        return 0;
    }

    TODO("HASH_APPEND FAILED");
    return -1;
}

#define HASHMAP_NO_VALUE_FOR_KEY -9999

long long int hashmap_getvalue_from_key(long long int key, HashMap *map)
{
    // returns -9999 in case it doesnt find key in hashmap
    long long int hash = hashkey(key);

    HashEntry *entry = &map->entries[hash];

    while(entry)
    {
        if(entry && entry->key==key) return entry->value;
        entry = entry->next;
    }


    return HASHMAP_NO_VALUE_FOR_KEY;

}

HashMap * hash_init()
{
    HashMap * m = (HashMap*)malloc(sizeof (HashMap));
    memset(m, 0, sizeof(HashMap));
    return m;
}

void print_arr(char *arr, int arr_size)
{
    for (int i = 0; i < arr_size; i++)
        printf("%c", *(arr + i));
}

#define MAX_ARRAY 10000000
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
    long long int possible_result = hashmap_getvalue_from_key(number, MAP);
    long long int left = number;
    if (possible_result !=  HASHMAP_NO_VALUE_FOR_KEY) return possible_result;

    int count = count_digits(number);

    for(int i = 0; i < count/2; i++)
        left/=10;

    hash_append(number, left, MAP);
    return left;
}

long long int right_half(long long int number)
{
    long long int result = number - left_half(number)*(pow(10, count_digits(number)/2));
    return result;
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
    MAP = hash_init();

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

    for(int it =0; it < 75; it++)
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

    printf("part1: %d", numbers.count); // p1: 175006
    return 0;
}