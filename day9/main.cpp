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
#define printf if(0) printf

int find_free_space()
{
    int i = 0;
    while(disk[i]!=-1) i++;
    return i;
}

int find_enough_free_space(int needed_size)
{

    int i = 0;
    while(1)
    {
        int found_size = 0;
        while(disk[i+found_size]==-1)
        {
            found_size++;
        }
        if (found_size >= needed_size)
        {
            printf("Found enough space (%d) at index %d\n",found_size, i);
            return i;
        }
        i++;
    }
    printf("Could not find enough space\n");
    return -1;
}

// returns value, and index, size by reference
int find_last_data_block(int &last_index, int &size)
{   

    while(disk[last_index]==-1){
        last_index--;
    }
    int data_value = disk[last_index];
    size = 0;

    while(disk[last_index]==data_value){
        last_index--;
        size++;
    }
    // last_index++; //roll back once to get last index at which disk[last_index] is data value

    printf("found last data %d block, from last_index %d with size %d\n", data_value, last_index, size);
    return data_value;

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
            }
            disk_curr_value += is_data ? 0 : 1;
            is_data=!is_data;
    }

}

void compact_disk_into_disk()
{
    int last_free_space = disk_len;
    while(1)
    {
        print_disk();

        int needed_size = 0;
        int value = find_last_data_block(last_free_space, needed_size);
        int free_space = find_enough_free_space(needed_size);


        if (last_free_space<=1)
           {printf("free_space after last_free_space, disk compacted as much as possible\n"); 
            return;}

        if (free_space > last_free_space) continue;

        printf("Compacting: Found free space at %d and used space at %d (size %d)  \n", free_space, last_free_space, needed_size);

        while(needed_size){
            printf("copying disk[%d] (%d) = disk[%d] (%d]);\n", free_space, disk[free_space], last_free_space+needed_size,disk[last_free_space + needed_size]);
            disk[free_space++]=disk[last_free_space+needed_size];
            disk[last_free_space+needed_size] = -1;
            needed_size--;
        }

        printf("Moved value %d (%d) from space %d to space %d with size %d\n", value, disk[free_space], last_free_space, free_space, needed_size);


    }
}

size_t get_disk_checksum()
{
    size_t checksum = 0;

    int i = 0;

    while (i < disk_len)
    {
        if (disk[i]!=-1 ) checksum+=i*disk[i];
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

    compact_disk_into_disk();

#undef printf
    printf("checksum = %llu",get_disk_checksum());
    return 0;
    }


/*
--- Day 9: Disk Fragmenter ---
Another push of the button leaves you in the familiar hallways of some friendly amphipods! Good thing you each somehow got your own personal mini submarine. The Historians jet away in search of the Chief, mostly by driving directly into walls.

While The Historians quickly figure out how to pilot these things, you notice an amphipod in the corner struggling with his computer. He's trying to make more contiguous free space by compacting all of the files, but his program isn't working; you offer to help.

He shows you the disk map (your puzzle input) he's already generated. For example:

2333133121414131402
The disk map uses a dense format to represent the layout of files and free space on the disk. The digits alternate between indicating the length of a file and the length of free space.

So, a disk map like 12345 would represent a one-block file, two blocks of free space, a three-block file, four blocks of free space, and then a five-block file. A disk map like 90909 would represent three nine-block files in a row (with no free space between them).

Each file on disk also has an ID number based on the order of the files as they appear before they are rearranged, starting with ID 0. So, the disk map 12345 has three files: a one-block file with ID 0, a three-block file with ID 1, and a five-block file with ID 2. Using one character for each block where digits are the file ID and . is free space, the disk map 12345 represents these individual blocks:

0..111....22222
The first example above, 2333133121414131402, represents these individual blocks:

00...111...2...333.44.5555.6666.777.888899
The amphipod would like to move file blocks one at a time from the end of the disk to the leftmost free space block (until there are no gaps remaining between file blocks). For the disk map 12345, the process looks like this:

0..111....22222
02.111....2222.
022111....222..
0221112...22...
02211122..2....
022111222......
The first example requires a few more steps:

00...111...2...333.44.5555.6666.777.888899
009..111...2...333.44.5555.6666.777.88889.
0099.111...2...333.44.5555.6666.777.8888..
00998111...2...333.44.5555.6666.777.888...
009981118..2...333.44.5555.6666.777.88....
0099811188.2...333.44.5555.6666.777.8.....
009981118882...333.44.5555.6666.777.......
0099811188827..333.44.5555.6666.77........
00998111888277.333.44.5555.6666.7.........
009981118882777333.44.5555.6666...........
009981118882777333644.5555.666............
00998111888277733364465555.66.............
0099811188827773336446555566..............
The final step of this file-compacting process is to update the filesystem checksum. To calculate the checksum, add up the result of multiplying each of these blocks' position with the file ID number it contains. The leftmost block is in position 0. If a block contains free space, skip it instead.

Continuing the first example, the first few blocks' position multiplied by its file ID number are 0 * 0 = 0, 1 * 0 = 0, 2 * 9 = 18, 3 * 9 = 27, 4 * 8 = 32, and so on. In this example, the checksum is the sum of these, 1928.

Compact the amphipod's hard drive using the process he requested. What is the resulting filesystem checksum? (Be careful copy/pasting the input for this puzzle; it is a single, very long line.)

Your puzzle answer was 6360094256423.

--- Part Two ---
Upon completion, two things immediately become clear. First, the disk definitely has a lot more contiguous free space, just like the amphipod hoped. Second, the computer is running much more slowly! Maybe introducing all of that file system fragmentation was a bad idea?

The eager amphipod already has a new plan: rather than move individual blocks, he'd like to try compacting the files on his disk by moving whole files instead.

This time, attempt to move whole files to the leftmost span of free space blocks that could fit the file. Attempt to move each file exactly once in order of decreasing file ID number starting with the file with the highest file ID number. If there is no span of free space to the left of a file that is large enough to fit the file, the file does not move.

The first example from above now proceeds differently:

00...111...2...333.44.5555.6666.777.888899
0099.111...2...333.44.5555.6666.777.8888..
0099.1117772...333.44.5555.6666.....8888..
0099.111777244.333....5555.6666.....8888..
00992111777.44.333....5555.6666.....8888..
The process of updating the filesystem checksum is the same; now, this example's checksum would be 2858.

Start over, now compacting the amphipod's hard drive using this new method instead. What is the resulting filesystem checksum?
*/