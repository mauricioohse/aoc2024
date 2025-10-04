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
#define MAX_SIZE 100
char line[MAX_SIZE] = {0};

// note: J and I start from 0
//100
//102
#define MAX_J 100 
#define MAX_I 102 

struct int2
{
    int64_t i;
    int64_t j;
};

struct Robot
{
    int2 p;
    int2 v;
} robots[500];

void print_robot(Robot r)
{
    printf("p:(%lld,%lld) v(%lld,%lld)\n", r.p.i, r.p.j, r.v.i, r.v.j);
}

int map[MAX_I+1][MAX_J+1] = {30};
int map_visited[MAX_I+1][MAX_J+1] = {0};

int is_valid(int i, int j)
{
    return (i >=0 && i <= MAX_I && j >=0 && j <= MAX_J );
}

int64_t iter = 0;

int dfs_count_neighbours(int i, int j, int sum)
{

    if (!is_valid(i,j)) return 0;

    if(map[i][j] == 0) return 0;

    if (map_visited[i][j] == 1)
        return 0;
    else
        map_visited[i][j] = 1;


    sum++;

    int2 dirs[] = {{1,0},{0,-1},{0,1},{-1,0},{-1,-1},{1,1},{-1,1},{1,-1} };

    for(int idx =0; idx < 8; idx++)
    {
        sum+=dfs_count_neighbours(i+dirs[idx].i,j+dirs[idx].j, sum);
    }

    iter++;
    //printf("curr sum:%d at iter %lld\n", sum, iter);
    return sum;

}

void build_map()
{
    memset(map, 0, sizeof(map));

    for (int i = 0; i < 500; i++)
    {
        int2 pos =  robots[i].p;

        map[pos.i][pos.j]++;
    }

}

void print_map()
{

    printf("\nstarting to print map:\n");
    for(int i = 0; i<(MAX_J+1)*(MAX_I+1); i++ )
    {
        printf("%d",*(&map[0][0]+i));
        if ( (i+1) %(MAX_J+1) ==0 ) printf("\n");
    }
}

int quadrants[4] = {0};

void add_to_quadrant(Robot robot)
{
    //print_robot(robot);
    int2 pos = robot.p;

    if (pos.i < 0 || pos.j < 0 || pos.i > MAX_I || pos.j > MAX_J)
        TODO("invalid position!\n");

    if (pos.i < MAX_I / 2 && pos.j < MAX_J / 2)
        quadrants[0]++;
    else if (pos.i < MAX_I / 2 && pos.j > MAX_J / 2)
        quadrants[1]++;
    else if (pos.i > MAX_I / 2 && pos.j > MAX_J / 2)
        quadrants[2]++;
    else if (pos.i > MAX_I / 2 && pos.j < MAX_J / 2)
        quadrants[3]++;
}

void apply_movement(Robot r, int seconds)
{
    Robot robot = r;
    robot.p.i += robot.v.i * seconds;
    robot.p.j += robot.v.j * seconds;

    // clamp
    robot.p.i = robot.p.i % (MAX_I+1);
    if (robot.p.i < 0)
        robot.p.i += MAX_I+1;

    robot.p.j = robot.p.j % (MAX_J+1);
    if (robot.p.j < 0)
        robot.p.j += MAX_J+1;

    map[robot.p.i][robot.p.j]++;
    add_to_quadrant(robot);
}

int main()
{

    FILE *f = fopen(INPUT, "r");
    int count = 0;
    while (fgets(line, sizeof(line), f))
    {
        sscanf(line, "p=%lld,%lld v=%lld,%lld", &robots[count].p.j, &robots[count].p.i, &robots[count].v.j, &robots[count].v.i);
        count++;
    }

    print_robot(robots[0]);
    print_robot(robots[count-1]);
    printf("count:%d\n", count);

    int max_sum = 0;
    uint64_t max_seconds = 0;

    for (uint64_t seconds = 0; seconds < 10000; seconds++)
    {
        memset(map, 0, sizeof(map));

        for (int i = 0; i < count; i++)
        {
            apply_movement(robots[i], seconds);
        }

        memset(map_visited, 0, sizeof(map_visited));

        for(int ii = 0; ii < MAX_I; ii++)
        for(int jj = 0; jj < MAX_J; jj++)
        {
            int sum = dfs_count_neighbours(ii,jj, 0);
            if ( sum > max_sum) {
                max_sum = sum;
                max_seconds = seconds;
                
            }
        }

        if (seconds == 7847) print_map();
        
    }

    printf("Quadrants sum: %d, %d, %d, %d\n", quadrants[0], quadrants[1], quadrants[2], quadrants[3]);
    printf("result = %lld\n", (int64_t)quadrants[0] * quadrants[1] * quadrants[2] * quadrants[3]);

    printf("max_sum %d max_seconds %lld", max_sum, max_seconds);

    return 0;
}