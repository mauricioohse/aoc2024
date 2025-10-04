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

#define WIDE_LEN (MAP_LEN*2)
char wide_map[WIDE_LEN][WIDE_LEN] ={0};
int wide_len = 0;

#define EMPTY         '.'
#define BOX_LEFT      '['
#define BOX_RIGHT     ']'


void swap(int i,int j, int di, int dj)
{
    char curr_ch = wide_map[i][j];
    int ni = i + di;
    int nj = j + dj;

    wide_map[i][j] = wide_map[ni][nj];
    wide_map[ni][nj] = curr_ch;
    
    if(wide_map[i][j] == '@')
    {
        ri = i;
        rj = j;
    }
    else if(wide_map[ni][nj] == '@')
    {
        ri = ni;
        rj = nj;
    }

}

int is_block (char ch)
{
    return (ch == '[' || ch == ']');
}

int move(int i, int j, int di, int dj)
{
    char curr_ch = wide_map[i][j];
    int ni = i + di;
    int nj = j + dj;
    char next_char = wide_map[ni][nj];

    if (curr_ch == '.')
    {
        return 1;
    }

    if (curr_ch == '@')
    {
        if (move(ni, nj, di, dj))
        {
            swap(i, j, di, dj);
            return 1;
        }
    }

    int moving_horizontal = (dj != 0);

    if (is_block(curr_ch) && moving_horizontal)
    {
        if (move(ni, nj, di, dj))
        {
            swap(i, j, di, dj);
            return 1;
        }
    }

    if (is_block(curr_ch) && !moving_horizontal)
    {
        int curr_candidate_to_move = is_block(next_char) || next_char == '.';
        int neigh_dj = (curr_ch == '[' ? 1 : -1);
        int neigh_i = i;
        int neigh_j = j + neigh_dj;
        int neighbour_candidate_to_move = 1; // todo
        if(curr_candidate_to_move && neighbour_candidate_to_move)
        {
            if(move(ni,nj,di,dj) && move(neigh_i + di , neigh_j + dj, di,dj))
            {   
                swap(i,j,di,dj);
                swap(neigh_i,neigh_j,di,dj);
                return 1;
            }
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



void create_wide_map()
{
    for (int i = 0; i < row_len; i++)
    {
        for (int j = 0; j < col_len; j++)
        {
            char ch1 = map[i][j] != 'O' ? map[i][j] : '[';
            char ch2 = map[i][j] != 'O' ? map[i][j] : ']';
            if (map[i][j] == '@')
            {
                ch2 = '.';
                ri = i;
                rj = 2*j;
            }

            wide_map[i][2 * j] = ch1;
            wide_map[i][2 * j + 1] = ch2;
        }
    }

    wide_len = 2 * row_len;
}

void print_wide_map()
{
    printf("wide_map:\n");
    for (int i = 0; i < row_len; i++)
    {
        for (int j = 0; j < wide_len; j++)
        {
            printf("%c", wide_map[i][j]);
        }
        printf("\n");
    }
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

    // transform map
    create_wide_map();
    print_wide_map();

    // move
    for (size_t i = 0; i < strlen(moves); i++)
    {
        int idx = ascii_to_dir(moves[i]);
        move( ri, rj, dirsi[idx], dirsj[idx]);

        printf("\nmove %lld (%c)\n",i, moves[i]);
        print_wide_map();

    }

    printf("\n robot finished moving:\n");
    print_wide_map();

    // calculate position sum
    size_t sum = 0;
    for(int i = 0; i < MAP_LEN; i++)
    for( int j  = 0; j < WIDE_LEN; j++)
    {
        if(wide_map[i][j] == BOX_LEFT)
            sum += i*100 + j;
    }

    printf("\nResult = %lld", sum); // 1577172: too low // 1570206 too low

    return 0;
}