#include <stdio.h>
#include <math.h> 
#include "string.h"


#define INPUT "input.txt"
// #define INPUT "test.txt"
#define MAP_MAX_SIZE 100
char map[MAP_MAX_SIZE*MAP_MAX_SIZE] = {0};
int map_size;

struct Point { int x,y;};
struct Points { Point data[500]; int size;};
Points visited_peaks;


int trail_scores[MAP_MAX_SIZE*MAP_MAX_SIZE] = {0};

void print_square_matrix(char * matrix, size_t size)
{
    for(size_t i = 0; i < size; i++ )
    {
        for (size_t j = 0; j < size; j++)
            printf("%c", matrix[i * size + j]);
        printf("\n");
    }
}

void print_square_matrix(int * matrix, size_t size)
{
    for(size_t i = 0; i < size; i++ )
    {
        for (size_t j = 0; j < size; j++)
            printf("%d ", matrix[i * size + j]);
        printf("\n");
    }
}

const int dirs[4][2]={{1,0},{-1,0},{0,1},{0,-1}};

int is_in_points(Points * points, Point point)
{
    for (int i = 0; i < points->size; i++)
        if (point.x == points->data[i].x && point.y == points->data[i].y)
            return 1;

    return 0;
}

int is_oob(Point p)
{
    return (p.x < 0 || p.x >= map_size || p.y < 0 || p.y >= map_size);
}

void run_trail(Point curr, Point start)
{
    if (is_oob(curr)) return;
    
    char curr_value = map[curr.x*map_size + curr.y];

    if (curr_value == '9')
    {
        Points *peak_data = &visited_peaks;
        if(!is_in_points(peak_data, curr))
        {
            trail_scores[curr.x * map_size + curr.y]++;
            peak_data->data[peak_data->size++] = curr;
            // printf("{%d,%d}\n", curr.x, curr.y);
            // print_square_matrix(trail_scores,map_size);
        }
        return;
    }

    for (int i = 0; i < 4; i++)
    {
        int new_x =(curr.x + dirs[i][0]);
        int new_y =(curr.y + dirs[i][1]);
        Point next = {new_x, new_y};
        if (is_oob(next)) continue;
        if (map[new_x*map_size + new_y] == curr_value+1)
            run_trail(next, start);
    }

}

int main()
{
    memset(&visited_peaks,0, sizeof(visited_peaks));

    freopen(INPUT, "r", stdin);

    int ch;
    int map_idx = 0;
    // Read character by character until the end of the file
    while ((ch = getchar()) != EOF)
    {
        // Only add characters that are part of the map, ignoring newlines
        if (ch >= '0' && ch <= '9')
        {
            map[map_idx++] = ch;
        }
    }

    map_size = (int)sqrt(map_idx);

    print_square_matrix(map, map_size);

    printf("starting trail analysis \n");

    for (int i = 0; i < map_size; i++)
        for (int j = 0; j < map_size; j++)
        {
            if (map[i*map_size + j] == '0') {
                visited_peaks.size=0;
                printf("starting trail {%d,%d}",i,j);
                run_trail(Point{i, j},Point{i, j});
                printf(" found %d peaks\n", visited_peaks.size);
            }
        }

    // summing scores
    print_square_matrix(trail_scores, map_size);
    
    size_t part1_score = 0;
    for (int i = 0; i < map_size; i++)
    for (int j = 0; j < map_size; j++)
    {
        part1_score += trail_scores[i * map_size + j];
    }

    printf("part1: %lld", part1_score);
    return 0;
}