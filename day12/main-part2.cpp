#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

// #pragma GCC diagnostic ignored "-Wunused-parameter" 
#pragma GCC diagnostic ignored "-Wwrite-strings"

#define TODO(text) {printf(text); exit(-1);}

#define INPUT "input.txt"
// #define INPUT "test.txt"
#define MAX_SIZE 141
char content[MAX_SIZE][MAX_SIZE] = {0};
int content_last_col = 0;
int content_last_row = 0;
int visited[MAX_SIZE][MAX_SIZE] = {0};

int dfs_area=0;
int dfs_perimeter=0;

struct Region {
    int area;
    int perimeter;
};

Region regions['Z'-'A'];
const int region_count = sizeof(regions)/sizeof(regions[0]);

int coord_is_valid(int x, int y)
{
    return (x>=0 && y >= 0 && x<content_last_col && y<content_last_col);
}


// dirs counter-clockwise order: RIGHT, UP, LEFT, DOWN
int dirsx[] = {{1}, {0}, {-1}, {0}};
int dirsy[] = {{0}, {1}, {0}, {-1}};
char *str_dir[] = {"DOWN", "RIGHT", "UP", "LEFT"};

int has_edge(int x, int y, char ch, int dir_idx)
{
    int nextx = x + dirsx[dir_idx];
    int nexty = y + dirsy[dir_idx];

    if(!coord_is_valid(nextx, nexty)) return 1;
    return ( ch != content[nextx][nexty]);
}

int has_unique_edge_in_dir(int x, int y, char ch, int dir_idx)
{
    // to check if a given edge is the "unique" edge in that side, we consider the following:
    // an edge is defined by a point and a direction. if the char in the direction position is different from the point char, then it is an edge.
    // an edge is an unique edge iff his counter-clockwise neighbour does not have an edge in the same direction

    if (!has_edge(x,y,ch,dir_idx)) return 0; 

    int neighbour_x = x+dirsx[(dir_idx+1)%4];
    int neighbour_y = y+dirsy[(dir_idx+1)%4];

    // if the neighbour is a different char, then it is an unique edge
    if (
        (coord_is_valid(neighbour_x, neighbour_y) && ch != content[neighbour_x][neighbour_y])
        || !coord_is_valid(neighbour_x, neighbour_y)
        ) return 1;

    // now check if the neighbour has an edge in the same dir: if yes, it is not unique edge.
    if(has_edge(neighbour_x, neighbour_y, ch, (dir_idx) )) return 0;

    return 1;
}

int how_many_edges(int x, int y, char ch)
{
    int edges = 0;

    for( int i = 0; i<4; i++)
    {
        edges+=has_unique_edge_in_dir(x,y,ch,i);
    }
    return edges;
}


void dfs_run_through_region(int x, int y, char ch)
{
    if(visited[x][y]) return;

    if(content[x][y] == ch)
    {
        visited[x][y] = 1;
        dfs_area++;
        dfs_perimeter+=how_many_edges(x,y,ch);
    }

    for (int i = 0; i < 4; i++)
    {
        int newx = x + dirsx[i];
        int newy = y + dirsy[i];
        if (!coord_is_valid(newx, newy))
        {
            continue;
        };

        if (ch != content[newx][newy])
            continue;

        if (ch == content[newx][newy])
            dfs_run_through_region( newx,  newy,  ch);
    }
}

int main()
{
    memset(regions, 0, sizeof(regions));

    FILE *f = fopen(INPUT, "r");
    int ch;
    while ((ch = fgetc(f)) != EOF)
    {
        if (ch != '\0' && ch != '\n' )
        {
            content[content_last_row][content_last_col++] = ch;
        }
        else
        {
            content_last_row++;
            content_last_col=0;
        }
    }
    content_last_row++;

    uint64_t ans = 0;
    // go by each i,j, calculate the number of edge and area for each region
    for(int i = 0;  i < content_last_row; i++)
    {
        for (int j = 0; j < content_last_col;j++)
        {
            if(!visited[i][j])
            {
                char ch = content[i][j];
                dfs_run_through_region(i,j,ch);

                ans+=dfs_area*dfs_perimeter;
                //printf("Region %c price %d area %d sides %d\n", ch, dfs_area*dfs_perimeter, dfs_area , dfs_perimeter);

                dfs_area = 0;
                dfs_perimeter = 0;

            }
        }
    }

    printf("result = %llu", ans);

    return 0;
}