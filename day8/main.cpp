#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int const LINE_SIZE = 200;
int const SIZE = 52;


#define RED_BEGIN "\033[1;31m"
#define RED_END   "\033[0m"

/* 
1) save each antena type 
    - for every antena, go through all other antenas of same type
        - CalcAntinodePositions(antena1,antena2,antinode1,antinode2)
        - add antinode(node) to antinodemap


*/
int new_line_or_end(char c){
    if(c == '\n') return true;
    if(c == '\0') return true;
    return false;
}

int is_antena(char c)
{
    if (c >= '0' && c <= 'z') return true;
    return false;
}
struct Pos{
    int x;
    int y;
};

struct antena_t{
    Pos pos[100];
    int antena_cnt;
    char c;
};

void add_antena(char c, Pos pos, antena_t *antena_data, int &antena_type_cnt){
    // add antena to antena_data
    int i = 0;
    do{
        if (antena_data[i].c == '\0')
        {
            // create instance data
            antena_data[i].c = c;
            antena_data[i].pos[0] = pos;
            antena_data[i].antena_cnt = 1;
            antena_type_cnt++;
            // printf("Added new antena type %c at pos (%d,%d), total types: %d\n", 
            //        c, pos.x, pos.y, antena_type_cnt);
            return;
        }
        else if (antena_data[i].c == c)
        {
            antena_data[i].pos[antena_data[i].antena_cnt++] = pos;
            // printf("Added antena %c at pos (%d,%d), total count for this type: %d\n",
            //        c, pos.x, pos.y, antena_data[i].antena_cnt);
            return;
        }
        i++;
    }while(i < antena_type_cnt+1);

}

void print_antena_data(antena_t* antena_data, int antena_type_cnt){
    for(int i = 0; i < antena_type_cnt; i++){
        printf("Antena type %c has %d instances at positions: ", antena_data[i].c, antena_data[i].antena_cnt);
        for(int j = 0; j < antena_data[i].antena_cnt; j++) {
            printf("(%d,%d) ", antena_data[i].pos[j].x, antena_data[i].pos[j].y);
        }
        printf("\n");
    }
}

void add_antinode(Pos n,char c, char *antinodes,int &antinode_count)
{
    if(antinodes[n.x*SIZE + n.y] =='\0')
    {
        antinodes[n.x*SIZE + n.y] = c;
        antinode_count++;
        printf("    Created antinode at position (%d,%d) with value %c\n", n.x, n.y, c);
    }
    else
    {
        // printf("    Attempted to create antinode at position (%d,%d) with value %c, but it already existed with value %c \n", n.x, n.y, c, antinodes[n.x*SIZE + n.y]);
    }
    
}

int antena_map_size = 0;

int is_valid_coord(Pos pos)
{
    return (pos.x >=0 && pos.x < antena_map_size &&
            pos.y >=0 && pos.y < antena_map_size);
}

void create_antinodes(Pos curr_pos, Pos comp_pos, char c , char *antinodes, int &antinode_count) // 632 too low
{
    int H = curr_pos.x - comp_pos.x;
    int L = curr_pos.y - comp_pos.y;

    Pos antinode1 = {curr_pos.x, curr_pos.y};
    Pos antinode2 = {comp_pos.x, comp_pos.y};

    // if(is_valid_coord(antinode1)) add_antinode(antinode1,c, antinodes, antinode_count);
    // if(is_valid_coord(antinode2)) add_antinode(antinode2,c, antinodes, antinode_count);

    while(is_valid_coord(antinode1))
    {
        add_antinode(antinode1,c, antinodes, antinode_count);
        antinode1 = {antinode1.x+H, antinode1.y+L};
    }

    while(is_valid_coord(antinode2))
    {
        add_antinode(antinode2,c, antinodes, antinode_count);
        antinode2 = {antinode2.x-H, antinode2.y-L};
    }


}
void create_antinode_map(antena_t* antena_data,int antena_type_cnt,char* antinodes, int &antinode_count)
{
    // iterate over antena data
    int a = 0;
    while(antena_data[a].c != '\0')
    {
        antena_t at = antena_data[a];
        printf("There are %d antena types, processing antena type %c with %d instances\n",antena_type_cnt, at.c, at.antena_cnt);
        // iterate from left to right in pairs of antenas of same type
        for (int i = 0; i < at.antena_cnt-1; i++){
            Pos curr_pos = at.pos[i];

            for (int j = i+1; j < at.antena_cnt ; j++){
                // compared antena
                Pos comp_pos = at.pos[j];
                create_antinodes(curr_pos, comp_pos, antena_data[a].c, antinodes, antinode_count);
            }
            
        }
        a++;
    }
}

void print_antena_and_antinode_data(char *antena_map,char  *antinodes,int size)
{
    for (int i = 0; i < size*size; i++)
    {
        char antinode = *(antinodes + i);
        char c = *(antena_map + i);
        if (antinode != '\0' ) printf(RED_BEGIN "%c" RED_END, c);
        else printf("%c",c);
    }
}


int main(){ 
    FILE* f = fopen("input.txt", "r");

    char antena_map[SIZE][SIZE] = {0};

    char antinodes[SIZE][SIZE] = {0};
    int antinode_count = 0;

    // antena data: holds all positions of antenas of certain type (determined by character value)
    antena_t antena_data[50];
    memset(antena_data,0,sizeof(antena_data));
    int antena_type_cnt = 0;

    char line[LINE_SIZE] = {0};
    int i = 0;

    while(fgets(line,LINE_SIZE-1,f))
    {
        snprintf(&antena_map[i][0],SIZE,line);
        printf("%s", &antena_map[i][0]);

        int j = 0;
        while(!new_line_or_end(antena_map[i][j]))
        {
            char c = antena_map[i][j];
            if(c!='.') printf("checking %c\n", c);
            if (is_antena(c))
                {
                    add_antena(c, Pos{i,j}, antena_data, antena_type_cnt);
                }
            j++;
        }

        i++;
    }

    antena_map_size = i;
    printf("\nantena map is %d long\n", antena_map_size);
    print_antena_data(antena_data, antena_type_cnt);


    create_antinode_map(antena_data,antena_type_cnt,antinodes[0], antinode_count);

    print_antena_and_antinode_data(antena_map[0], antinodes[0], SIZE);

    // int valid_eq_sum = run_part2(equations, eq_cnt);

    printf("\nPart2 result %d antinodes\n", antinode_count);

    return 0;
    }

    /*
    --- Day 8: Resonant Collinearity ---
You find yourselves on the roof of a top-secret Easter Bunny installation.

While The Historians do their thing, you take a look at the familiar huge antenna. Much to your surprise, it seems to have been reconfigured to emit a signal that makes people 0.1% more likely to buy Easter Bunny brand Imitation Mediocre Chocolate as a Christmas gift! Unthinkable!

Scanning across the city, you find that there are actually many such antennas. Each antenna is tuned to a specific frequency indicated by a single lowercase letter, uppercase letter, or digit. You create a map (your puzzle input) of these antennas. For example:

............
........0...
.....0......
.......0....
....0.......
......A.....
............
............
........A...
.........A..
............
............
The signal only applies its nefarious effect at specific antinodes based on the resonant frequencies of the antennas. In particular, an antinode occurs at any point that is perfectly in line with two antennas of the same frequency - but only when one of the antennas is twice as far away as the other. This means that for any pair of antennas with the same frequency, there are two antinodes, one on either side of them.

So, for these two antennas with frequency a, they create the two antinodes marked with #:

..........
...#......
..........
....a.....
..........
.....a....
..........
......#...
..........
..........
Adding a third antenna with the same frequency creates several more antinodes. It would ideally add four antinodes, but two are off the right side of the map, so instead it adds only two:

..........
...#......
#.........
....a.....
........a.
.....a....
..#.......
......#...
..........
..........
Antennas with different frequencies don't create antinodes; A and a count as different frequencies. However, antinodes can occur at locations that contain antennas. In this diagram, the lone antenna with frequency capital A creates no antinodes but has a lowercase-a-frequency antinode at its location:

..........
...#......
#.........
....a.....
........a.
.....a....
..#.......
......A...
..........
..........
The first example has antennas with two different frequencies, so the antinodes they create look like this, plus an antinode overlapping the topmost A-frequency antenna:

......#....#
...#....0...
....#0....#.
..#....0....
....0....#..
.#....A.....
...#........
#......#....
........A...
.........A..
..........#.
..........#.
Because the topmost A-frequency antenna overlaps with a 0-frequency antinode, there are 14 total unique locations that contain an antinode within the bounds of the map.

Calculate the impact of the signal. How many unique locations within the bounds of the map contain an antinode?

Your puzzle answer was 269.

--- Part Two ---
Watching over your shoulder as you work, one of The Historians asks if you took the effects of resonant harmonics into your calculations.

Whoops!

After updating your model, it turns out that an antinode occurs at any grid position exactly in line with at least two antennas of the same frequency, regardless of distance. This means that some of the new antinodes will occur at the position of each antenna (unless that antenna is the only one of its frequency).

So, these three T-frequency antennas now create many antinodes:

T....#....
...T......
.T....#...
.........#
..#.......
..........
...#......
..........
....#.....
..........
In fact, the three T-frequency antennas are all exactly in line with two antennas, so they are all also antinodes! This brings the total number of antinodes in the above example to 9.

The original example now has 34 antinodes, including the antinodes that appear on every antenna:

##....#....#
.#.#....0...
..#.#0....#.
..##...0....
....0....#..
.#...#A....#
...#..#.....
#....#.#....
..#.....A...
....#....A..
.#........#.
...#......##
Calculate the impact of the signal using this updated model. How many unique locations within the bounds of the map contain an antinode?

Your puzzle answer was 949.
    */