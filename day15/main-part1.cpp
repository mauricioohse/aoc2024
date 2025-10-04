#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

// #pragma GCC diagnostic ignored "-Wunused-parameter"
// #pragma GCC diagnostic ignored "-Wwrite-strings"
#define TODO(text)    \
    {                 \
        printf(text); \
        exit(-1);     \
    }

#define INPUT "input.txt"
// #define INPUT "test.txt"
#define MAP_LEN 52

#define LINE_SIZE 2000
char line[LINE_SIZE] = {0};

char map[MAP_LEN][MAP_LEN] = {0};
int row_len = 0, col_len = 0;

#define MOVES_SIZE 30000
char moves[MOVES_SIZE] = {0};
int moves_count = 0;

// robot
int ri = 0;
int rj = 0;

struct int2
{
    int64_t i;
    int64_t j;
};

void print_map()
{
    for(int i = 0 ; i < row_len ; i++)
    {
        for (int j = 0; j < col_len; j++)
        {
            printf("%c", map[i][j]);
        }
        printf("\n");
    }
    printf("robot pos: (%d,%d) proof: @ == %c \n", ri, rj, map[ri][rj]);
}

void print_moves()
{
    char *m = moves;
    while(*m != '\0')
    {
        printf("%c", *m);
        m++;
    }
}

int is_valid(int i, int j)
{
    return(i>0 && i < MAP_LEN && j>0 && j < MAP_LEN);
}

#define EMPTY   '.'
#define BOX     'O'
// returns 1 when successful, 0 otherwise
int attempt_move(char ch, int i, int j,int di, int dj)
{
    int ni = i + di;
    int nj = j + dj;

    if(!is_valid(ni,nj)) return 0;

    if(map[ni][nj]==EMPTY)
    {
        // can move
        map[ni][nj] = ch;
        map[i][j] = EMPTY;
        return 1;
    }
    else if(map[ni][nj]==BOX)
    {
        if (attempt_move(BOX, ni, nj, di, dj))
        {
            // can move
            map[ni][nj] = ch;
            map[i][j] = EMPTY;
            return 1;
        }
    }

    return 0;
}

//                DOWN, UP, RIGHT, LEFT
const int dirsi[] = {1, -1, 0, 0};
const int dirsj[] = {0, 0, 1, -1};
char ascii_to_dir(char ch)
{
    switch (ch)
    {
    case '^':
        return 1;
        break;
    case 'v':
        return 0;
        break;
    case '>':
        return 2;
        break;
    case '<':
        return 3;
        break;
    
    default:
        TODO("ERROOOOOOR");
        break;
    }
    TODO("ERRRRRRRROO");
    return 10;
}

int main()
{

    FILE *f = fopen(INPUT, "r");
    int map_finished = 0;
    
    char ch;
    while( ( ch = fgetc(f)) != EOF)
    {
        if (ch == '<' || ch == '^')
            map_finished = 1;

        if(!map_finished)
        {
            if (ch == '\n')
            {
                col_len = 0;
                row_len++;
            }
            else
            {
                map[row_len][col_len++] = ch;
                if(ch == '@')
                {
                    ri = row_len;
                    rj = col_len-1;
                }
            }
        }
        else
        {
            if (ch != '\n')
                moves[moves_count++] = ch;
        }
    }
    //row_len++;
    col_len = row_len;

    // prints
    print_map();
    print_moves();

    // move
    for (size_t i = 0; i < strlen(moves); i++)
    {
        int idx = ascii_to_dir(moves[i]);
        if (attempt_move('@', ri, rj, dirsi[idx], dirsj[idx]))
        {
            ri = ri + dirsi[idx];
            rj = rj + dirsj[idx];
        }

        // printf("\nmove %lld (%c)\n",i, moves[i]);
        //print_map();

    }

    printf("\n robot finished moving:\n");
    print_map();

    // calculate position sum
    size_t sum = 0;
    for(int i = 0; i < MAP_LEN; i++)
    for( int j  = 0; j < MAP_LEN; j++)
    {
        if(map[i][j] == BOX)
            sum += i*100 + j;
    }

    printf("\nResult = %lld", sum);  // too low: 1562778

    return 0;
}