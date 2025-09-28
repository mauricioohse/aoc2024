#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#pragma GCC diagnostic ignored "-Wunused-parameter" 

#define TODO(text) {printf(text); exit(-1);}

#define INPUT "input.txt"
// #define INPUT "test.txt"
#define MAX_SIZE 100
char content[MAX_SIZE] = {0};
int content_count = 0;

struct Key
{
    int64_t number;
    int64_t blinks;
};


struct HashEntry
{
    Key key;
    int64_t hash;
    int64_t stone_count; // result of the left_part() function
    HashEntry *next;
};

#define HASH_MAP_SIZE (1<<20)

struct HashMap
{
    HashEntry entries[HASH_MAP_SIZE+2];
};

HashMap *MAP;

int64_t hashkey(Key key)
{
    return (key.number%HASH_MAP_SIZE +1 + key.blinks);
}

int is_key_equal(Key k1, Key k2)
{
    return (k1.blinks==k2.blinks && k2.number == k1.number);
}

int64_t hash_append(int64_t number, int64_t stone_count, int16_t blinks,  HashMap * map)
{
    // return 0 on success, 1 in case entry was already found
    Key key = Key{number, blinks};
    int64_t hash = hashkey(key);

    HashEntry *entry = &map->entries[hash];
    // assume hashmap was initialized with nullptr in last hash_entry
    while(entry->next!=nullptr)
    {
        if(is_key_equal( entry->key, key)) return 1; // we already had the entry, just return
        entry = entry->next;
    }

    if (entry->next== nullptr)
    {
        // we didnt find the entry already, so now we append before the last entry
        HashEntry *new_entry = (HashEntry *)malloc(sizeof(HashEntry));
        memset(new_entry,0, sizeof(HashEntry));

        entry->key = key;
        entry->stone_count = stone_count;
        entry->next = new_entry;
        //printf("appended key {%lld, %lld} stone_count %lld at hash %lld\n", key.number, key.blinks, stone_count, hash);
        return 0;
    }

    TODO("HASH_APPEND FAILED");
    return -1;
}

#define HASHMAP_DIDNT_FIND_STONECOUNT -1

int64_t hashmap_getstone_count_from_key(Key key, HashMap *map)
{
    int64_t hash = hashkey(key);
    HashEntry *entry = &map->entries[hash];

    while(entry)
    {
        if(entry && is_key_equal(entry->key,key)) return entry->stone_count;
        entry = entry->next;
    }

    return HASHMAP_DIDNT_FIND_STONECOUNT;
}

HashMap * hash_init()
{
    HashMap * m = (HashMap*)malloc(sizeof (HashMap));
    memset(m, 0, sizeof(HashMap));
    return m;
}

struct Array
{
    int64_t items[20];
    int count;
} numbers;

int count_digits(int64_t number)
{
    int count = 0;
    while(number!=0)
    {
        number = number/10;
        count++;
    }
    return count;
}

int64_t left_half(int64_t number, int64_t blinks)
{
    int64_t left = number;
    int count = count_digits(number);

    for(int i = 0; i < count/2; i++)
        left/=10;

    return left;
}

int64_t ipow10(int i)
{
    const int64_t result[] = 
    {
        1LL,
        10LL,
        100LL,
        1000LL,
        10000LL,
        100000LL,
        1000000LL,
        10000000LL,
        100000000LL,
        1000000000LL,
        10000000000LL,
        100000000000LL,
        1000000000000LL,
        10000000000000LL,
        100000000000000LL,
        1000000000000000LL
    };

    int result_count = sizeof(result)/sizeof(result[0]);

    if( i<0 || i>result_count) {printf("ipow10 ERROR: i=%d, result_count %d", i, result_count); exit(-1);}

    return result[i];
}

int64_t right_half(int64_t number, int64_t blinks)
{
    int64_t result = number - left_half(number, blinks)*(ipow10(count_digits(number)/2));
    return result;
}

// return the number of stones for a given number,blink pair
int64_t Calculate_items(int64_t number, int64_t blinks)
{
    int64_t result = 0;
    if (blinks == 0) return 1;

    int64_t partial_result = hashmap_getstone_count_from_key(Key{number, blinks}, MAP);

    if (partial_result != HASHMAP_DIDNT_FIND_STONECOUNT)
        return partial_result;

    if (number == 0)
    {
        result += Calculate_items(1, blinks - 1);
        return result;
    }
    else if (count_digits(number) % 2 == 0)
    {
        int64_t right = right_half(number, blinks);
        int64_t left = left_half(number, blinks);
        result += Calculate_items(right, blinks - 1);
        result += Calculate_items(left, blinks - 1);
        hash_append(number, result, blinks, MAP);
    }
    else
    {
        result += Calculate_items(number * 2024ll, blinks - 1);
        hash_append(number, result, blinks, MAP);
    }

    return result;
}

int main()
{
    memset(&numbers, 0, sizeof(numbers));
    MAP = hash_init();

    FILE *f = fopen(INPUT, "r");
    int ch;
    while ((ch = fgetc(f)) != EOF)
    {
        if (ch != '\0')
        {
            content[content_count++] = ch;
        }
    }

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

    printf("\n");

    int64_t sum = 0;
    for (int64_t i = 0; i < numbers.count; i++)
    {
        sum+=Calculate_items(numbers.items[i], 75);
    }


    printf("Answer: %lld", sum);
    return 0;
}