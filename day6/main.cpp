#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include <windows.h>

#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define RED_BEGIN "\033[1;31m"
#define RED_END   "\033[0m"

using namespace std;


void print_arr(char *arr, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        printf("%c", arr[i]);
    }
}

void print_arr(size_t *arr, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        printf("%llu ", arr[i]);
    }
}

const size_t MAP_SIZE = 150;
char original_map[MAP_SIZE][MAP_SIZE];
size_t max_xy = 0;

static const int dirs[][2] = {{-1,0}, {0,1}, {1,0}, {0,-1}};
enum DIR{
    UP,
    RIGHT,
    DOWN,
    LEFT
};

void move_pos_by_dir(size_t* pos, DIR dir){
    pos[0] += dirs[dir][0];
    pos[1] += dirs[dir][1];
}

struct guard_t{
    size_t pos[2] = {0,0};
    DIR current_dir = DOWN;


    void move(){
        move_pos_by_dir(pos, current_dir);
    }

    char get_next_tile(size_t *rtn_pos, char* m){
        rtn_pos[0]=pos[0] + dirs[current_dir][0];
        rtn_pos[1]=pos[1] + dirs[current_dir][1];

        return (m[rtn_pos[0]* MAP_SIZE + rtn_pos[1]]);
    }

    void rotate(){
        current_dir = (DIR)((current_dir + 1) % 4);
    }

};

guard_t guard;

size_t path_positions[6000][2];
size_t path_cnt = 1;


void save_guard_pos(DIR dir, size_t i, size_t row)
{
    guard.current_dir = dir;
    guard.pos[1] = i;
    guard.pos[0] = row;
    original_map[row][i] = 'X';
    printf("guard pos(%lld,%lld) and dir(%d,%d) found!\n",
           guard.pos[0], guard.pos[1], dirs[guard.current_dir][0], dirs[guard.current_dir][1]);
    path_positions[0][0] = guard.pos[0];
    path_positions[0][1] = guard.pos[1];

    printf("path position (%llu,  %llu)", path_positions[0][0], path_positions[0][1]);
}

void find_guard_pos_and_dir(char *map_row, size_t row)
{
    for (size_t i = 0; i < max_xy; i++)
    {
        if ( map_row[i]=='^' )
        {
            save_guard_pos(UP, i, row);
        }

    }
}


size_t run_map_part1(char *m)
{
    size_t path_idx = 1;
    int guard_is_out_of_bounds = false; 
    size_t X_count = 1;

    while(!guard_is_out_of_bounds)
    {
        size_t next_tile_pos[2] = {0};
        char next_tile = guard.get_next_tile(next_tile_pos, m);
        size_t n_x = next_tile_pos[0];
        size_t n_y = next_tile_pos[1];

        if (n_x >= max_xy || n_y > max_xy )
        {
            guard_is_out_of_bounds = true;
            break;
        }

        switch (next_tile)
        {
        case '.':
        case '^':
            m[n_x*MAP_SIZE + n_y] = 'X';
            X_count++;
            // no break on purpose
            guard.move();
            path_positions[path_idx][0] = guard.pos[0];
            path_positions[path_idx++][1] = guard.pos[1];
            path_cnt++;
            break;
        case 'X':
            guard.move();
            break;
        case '#':
            guard.rotate();
            break;
        case '\n':
            guard_is_out_of_bounds = true;
            break;
        default:
            printf("\nWRONG SYMBOL FOUND %c \n", m[n_x*MAP_SIZE + n_y]);
        }

        
        // printf("\n guard pos(%llu, %llu) - max_xy %llu - moved to (%llu,%llu) - cnt %llu \n", 
        //     guard.pos[0], guard.pos[1], max_xy, n_x, n_y, X_count);
        // // for (size_t i =0 ; i < max_xy; i++)
        // // {
        // //     print_arr(&original_map[i*MAP_SIZE], max_xy);
        // //     printf("\n");
        // // }

        // printf("\n");
        // Sleep(50);
    }

    printf("\n some data: path_idx %llu, x_count %llu",path_idx, X_count);
    return X_count;
}


size_t run_map_part2(char *m)
{

    int guard_is_out_of_bounds = false; 
    size_t step_count = 1;

    while(!guard_is_out_of_bounds)
    {
        size_t next_tile_pos[2] = {0};
        char next_tile = guard.get_next_tile(next_tile_pos, m);
        size_t n_x = next_tile_pos[0];
        size_t n_y = next_tile_pos[1];

        if (n_x >= max_xy || n_y > max_xy )
        {
            guard_is_out_of_bounds = true;
            break;
        }

        switch (next_tile)
        {
        case '.':
        case '^':
            m[n_x*MAP_SIZE + n_y] = 'X';
            step_count++;
            // no break on purpose
            guard.move();
            break;
        case 'X':
            guard.move();
            step_count++;
            break;
        case '#':
            guard.rotate();
            break;
        case '\n':
            guard_is_out_of_bounds = true;
            break;
        default:
            printf("\nWRONG SYMBOL FOUND %c \n", m[n_x*MAP_SIZE + n_y]);
        }

        if (step_count> 25000) 
        {
            // printf("\n guard pos(%llu, %llu) - max_xy %llu - moved to (%llu,%llu) - step_count %llu \n",
            //        guard.pos[0], guard.pos[1], max_xy, n_x, n_y, step_count);
            // for (size_t i =0 ; i < max_xy; i++)
            // {
            //     print_arr(&m[i*MAP_SIZE], max_xy);
            //     printf("\n");
            // }

            // printf("\n");

            break;
        }
        // Sleep(50);
    }

    volatile static int debug =0;
    if (debug)
    for (size_t i = 0; i < max_xy; i++)
    {
        print_arr(&m[i * MAP_SIZE], max_xy);
        printf("\n");
    }

    return (step_count > 25000)? true: false;
}

int main(){

    memset(original_map,0,sizeof(original_map));
    memset(path_positions,0,sizeof(path_positions));

    FILE* f = fopen("input.txt", "r");

    char line[MAP_SIZE-1] = {0};
    size_t i = 0;

    while(fgets(line,MAP_SIZE-1,f))
    {
        strcpy(&original_map[i][0], line);
        
        max_xy = strlen(line);        
        print_arr(&original_map[i][0], max_xy);

        find_guard_pos_and_dir(&original_map[i][0], i);
        guard_t original_guard;

        original_guard.pos[0] = guard.pos[0];
        original_guard.pos[1] = guard.pos[1]; 
        original_guard.current_dir = guard.current_dir;

        i++;
    }

    char part1_map[MAP_SIZE][MAP_SIZE];
    memset(part1_map,0,sizeof(part1_map));
    memcpy(part1_map,original_map,sizeof(part1_map));

    size_t X_count = run_map_part1(part1_map[0]);


    printf("\nPart1 result1 %lld\n", X_count);
    printf(" ------------- Part2 starting ------------- \n\n\n");

    print_arr(path_positions[0],path_cnt*2);
    printf("\n");

    // part2
    size_t loop_count = 0;
    size_t path_idx = 1;

    while (path_positions[path_idx][1]!=0 || path_positions[path_idx][0]!=0 )
    {


        char part2_map[MAP_SIZE][MAP_SIZE];
        memset(part2_map,0,sizeof(part2_map));
        memcpy(part2_map,original_map,sizeof(part2_map));

        size_t block_x = path_positions[path_idx][0];
        size_t block_y = path_positions[path_idx][1];
        guard.pos[0] =  path_positions[0][0];
        guard.pos[1] =  path_positions[0][1];
        guard.current_dir = UP;


        if (path_idx==5028)
            printf("here\n");
        part2_map[block_x][block_y] = '#';
        size_t change = run_map_part2(part2_map[0]);
        loop_count += change;
        path_idx++;


        if (1) // change
            printf("continue loop? (%d), path_idx %llu,  count:  %llu , current block position: (%llu,  %llu), next: (%llu,  %llu)\n",
                    path_positions[path_idx][1]!=0 || path_positions[path_idx][0]!=0 , path_idx,
                    loop_count, block_x, block_y, path_positions[path_idx][0], path_positions[path_idx][1]);
    }
    printf("Part2 result %lld\n", loop_count);

    return 0;
    }