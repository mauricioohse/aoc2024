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
        printf("    Attempted to create antinode at position (%d,%d) with value %c, but it already existed with value %c \n", n.x, n.y, c, antinodes[n.x*SIZE + n.y]);
    }
    
}

int antena_map_size = 0;

int is_valid_coord(Pos pos)
{
    return (pos.x >=0 && pos.x < antena_map_size &&
            pos.y >=0 && pos.y < antena_map_size);
}

void create_antinodes(Pos curr_pos, Pos comp_pos, char c , char *antinodes, int &antinode_count)
{
    int H = curr_pos.x - comp_pos.x;
    int L = curr_pos.y - comp_pos.y;

    Pos antinode1 = {curr_pos.x+H, curr_pos.y+L};
    Pos antinode2 = {comp_pos.x-H, comp_pos.y-L};

    if(is_valid_coord(antinode1)) add_antinode(antinode1,c, antinodes, antinode_count);
    if(is_valid_coord(antinode2)) add_antinode(antinode2,c, antinodes, antinode_count);

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