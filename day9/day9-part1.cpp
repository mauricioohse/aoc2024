#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <windows.h>

char content[20000] = {0};
int content_len = 0;

int const disk_size = 180000;
int disk[disk_size] = {0};
int disk_len = 0;

int find_free_space()
{
    int i = 0;
    while(disk[i]!=-1) i++;
    return i;
}

int find_enough_free_space(int needed_size)
{
    int i = 0;
    while(disk[i]!=-1)
    {
        i++;
    }
    return i;
}

void print_disk()
{
    for (int i = 0; i < disk_len; i++)
    {
        printf("%d ", disk[i]);
    }
    printf("\n");
}

int find_last_used_space()
{
    int i = disk_size-1;
    while(disk[i]==-1) i--;
    return i;
}

void expand_content_into_disk()
{
    int disk_curr_value = 0;
    int is_data = 1;
    for (int i = 0; i < content_len; i++)
    {

            for (int j = 0; j < content[i] - '0'; j++)
            {
                disk[disk_len++] = is_data ? disk_curr_value : -1;
                if (is_data)
                {
                    // printf("%d ", disk_curr_value);
                }
                else
                {
                    // printf(".");
                }
            }
            disk_curr_value += is_data ? 0 : 1;
            is_data=!is_data;
    }

}

void compact_disk_into_disk()
{
    while(1)
    {
        int free_space = find_free_space();
        int used_space = find_last_used_space();

        if (free_space > used_space)
            return;

        disk[free_space] = disk[used_space];
        disk[used_space] = -1;

        // printf("Moving %d from space %d to space %d\n", disk[free_space], used_space, free_space);
        // // print_disk();
        // Sleep(100);
    }
}

size_t get_disk_checksum()
{
    size_t checksum = 0;

    int i = 0;

    while (disk[i]!=-1)
    {
        checksum+=i*disk[i];
        i++;
    }
    return checksum;
}

int main(){ 

    memset(disk,-1,sizeof(disk));

    FILE* f = fopen("input.txt", "r");

    char c = 0;
    int i = 0;
    while((c = getc(f)) != EOF)
    {
        content[i++] = c;
        // printf("%c",c);
    }
    content_len = i;

    expand_content_into_disk();

    // print_disk();

    compact_disk_into_disk();

    // print_disk();

    printf("checksum = %llu",get_disk_checksum());
    // size_t eq_cnt = i;
    // size_t valid_eq_sum = run_part2(equations, eq_cnt);

    // printf("\nPart2 result %lld\n", valid_eq_sum);

    return 0;
    }