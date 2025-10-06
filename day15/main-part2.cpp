#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#define RED_BEG "\033[0;31m"
#define RED_END "\033[0m"

#define INPUT "input.txt"
#define MAP_LEN 52

char map[MAP_LEN][MAP_LEN] = {0};
int row_len = 0, col_len = 0;

#define MOVES_SIZE 30000
char moves[MOVES_SIZE] = {0};
int moves_count = 0;

// robot coords
int ri = 0;
int rj = 0;


struct int2
{
    int64_t i;
    int64_t j;
};

#define STACK_MAX_SIZE 50
struct Stack
{
    int2 coord[STACK_MAX_SIZE];
    int count;
} stack;

void st_append(Stack *s, int i, int j)
{
    if(s->count >= STACK_MAX_SIZE){ printf("alloc more STACK_SIZE!"); return;}

    // check if duplicate
    for(int idx = 0; idx < stack.count; idx ++)
        if( stack.coord[idx].i == i && stack.coord[idx].j == j) return;

    s->coord[s->count].i = i;
    s->coord[s->count++].j = j;  

}

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

void print_n_moves(int n)
{
    printf("printing moves (%d):", n);

    char *m = moves;
    for( int i = 0; i < n; i++)
    {
        printf("%c", *m);
        m++;
    }
    printf("\n");
}

void print_moves()
{
    printf("printing moves: %d\n", moves_count);

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
int visited[WIDE_LEN][WIDE_LEN] = {0};

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
    visited[i][j] = 1;
    visited[ni][nj] = 1;
    
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

int comp_dir_i = 0;
int comp_dir_j = 0;
int comp_func(const void* first, const void * second)
{
    int2 *f = (int2*)first;
    int2 *s = (int2*)second;

    if(comp_dir_i == 1)     return f->i - s->i;
    if(comp_dir_i == -1)    return s->i - f->i;
    if(comp_dir_j == 1)     return f->j - s->j;
    if(comp_dir_j == -1)    return s->j - f->j;

    return 0;
}

void swap_stack(Stack * stack, int di, int dj)
{
    comp_dir_i = di;
    comp_dir_j = dj;
    qsort(&stack->coord, stack->count, sizeof(int2), comp_func);

    for (int i = stack->count-1; i >=0 ; i--)
    {
        swap(stack->coord[i].i, stack->coord[i].j, di, dj);
    }

}

int is_block (char ch)
{
    return (ch == '[' || ch == ']');
}

int is_movable(char ch)
{
    return (ch == '[' || ch == ']' || ch =='.');
}

int can_move(int i, int j, int di, int dj)
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
        if (is_movable(next_char))
        {
            st_append(&stack, i, j);
            return (can_move(ni, nj, di, dj));
        }
    }

    int moving_horizontal = (dj != 0);

    if (is_block(curr_ch) && moving_horizontal)
    {
        if (is_movable(next_char))
        {
            st_append(&stack, i, j);
            return can_move(ni, nj, di, dj);
        }
    }

    if (is_block(curr_ch) && !moving_horizontal)
    {
        int neigh_dj = (curr_ch == '[' ? 1 : -1);
        int neigh_i = i;
        int neigh_j = j + neigh_dj;
        char neighbour_next_ch = wide_map[neigh_i + di][neigh_j + dj];
        if (is_movable(next_char) && is_movable(neighbour_next_ch))
        {
            st_append(&stack, i, j);
            st_append(&stack, neigh_i, neigh_j);
            return can_move(ni, nj, di, dj) && can_move(neigh_i + di, neigh_j + dj, di, dj);
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
        printf("direction '%c' not expected!\n", ch);
        return 0;
        break;
    }
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
            if(visited[i][j] == 1) printf(RED_BEG);
            printf("%c", wide_map[i][j]);
            if(visited[i][j] == 1) printf(RED_END);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[])
{
    memset(&stack, 0, sizeof(stack));
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
    col_len = row_len;

    // transform map
    create_wide_map();

    // can_move
    for (size_t i = 0; i < strlen(moves); i++)
    {
        int idx = ascii_to_dir(moves[i]);

        if (argc > 2 && i == strtod(argv[1], NULL))
        {   // used for debugging...
            printf("\nmove %lld (%c)\n", i, moves[i]);
            print_wide_map();
        }

        stack.count = 0;
        if (can_move(ri, rj, dirsi[idx], dirsj[idx]))
        {
            swap_stack(&stack, dirsi[idx], dirsj[idx]);
        }
        memset(visited, 0, sizeof(visited));
    }

    printf("\n robot finished moving:\n");

    // calculate position sum
    size_t sum = 0;
    for(int i = 0; i < MAP_LEN; i++)
    for( int j  = 0; j < WIDE_LEN; j++)
    {
        if(wide_map[i][j] == BOX_LEFT)
            sum += i*100 + j;
    }

    printf("\nResult = %lld", sum);

    return 0;
}