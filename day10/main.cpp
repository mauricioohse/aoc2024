#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>



struct Pos{
    int x;
    int y;

    Pos operator+(const Pos &other) const { return Pos{x + other.x, y + other.y}; }
}; 

struct Trailhead{
    Pos pos;
    Pos peaks_reached[50];
    int peaks_count;
};

struct Trailheads{
    Trailhead trailheads[200];
    int trailhead_count;
};

// globals
char mapp[57][57] = {0};
int mapsize = 0;
Trailheads trails;
int reached_peaks_count = 0;

void add_trailhead(Pos p)
{
    trails.trailheads[trails.trailhead_count++].pos=p;
}

int mark_as_reached(int trail_id, Pos peak_pos)
{   // returns 1 when sucessfully marking, 0 otherwise
    Trailhead t = trails.trailheads[trail_id];
    int max_peaks = t.peaks_count;

    for (int i = 0; i <= max_peaks; i++)
    {
        if ( !memcmp(&trails.trailheads[trail_id].peaks_reached[i], &peak_pos, sizeof(Pos))) return 0; // was already marked
    }

    // was not marked yet, so mark it
    trails.trailheads[trail_id].peaks_reached[trails.trailheads[trail_id].peaks_count++]=peak_pos;
    reached_peaks_count++;
    return 1;

}

int dfs( int mapsize, int trail_id, Pos p, int sum){

    if (p.x <0 || p.x > mapsize || p.y <0 || p.y > mapsize) return 0;

    // printf("checking {%d,%d} c= %c\n", p.x,p.y, mapp[p.x][p.y] );

    if (mapp[p.x][p.y] == '9') {
        // peak reached, attempt to mark as reached 
        if( mark_as_reached(trail_id, p))
            return 1;
        else
            return 0; // was already marked
    }

    // actual DFS
    Pos dirs[] = {{1,0},{-1,0},{0,1},{0,-1}};
    for(int i = 0 ; i < 4; i++)
    {
        Pos new_pos = p + dirs[i];
        
        if (new_pos.x <0 || new_pos.x > mapsize || new_pos.y <0 || new_pos.y > mapsize) 
            continue;

        // printf("checking new_pos{%d,%d} c= %c\n",new_pos.x,new_pos.y, mapp[new_pos.x][new_pos.y]);
        if (mapp[new_pos.x][new_pos.y] == (mapp[p.x][p.y] + 1) ) // condition: 1 more height each step
            sum+=dfs( mapsize, trail_id, p+dirs[i], sum);

    }

    return sum;
}



int main(){  

    memset(&trails,0,sizeof(trails));
    FILE* f = fopen("input.txt", "r");
    int i = 0;

    int j = 0;
    char c = 0;
    while ((c = getc(f)) != EOF)
    {
        if (c== '0')
            add_trailhead(Pos{i, j});
        mapp[i][j] = c;
        printf("%c",c);

        if (c == '\n')
        {
            j = 0;
            i++;
        }
        else
            j++;
    }
    mapsize = i;

    long sum = 0;

    // run dfs through every trailhead
    for (int t = 0; t < trails.trailhead_count; t++)
    {
        sum+=dfs( mapsize, t, trails.trailheads[t].pos, 0);
    }

    printf("\nPart1 result %ld\n", sum);
    printf("\nPart1 result %d\n", reached_peaks_count+1);

    int peak_count = 0;
    // goes through reached peaks
    for (int t=0 ; t< trails.trailhead_count ; t++){
        Trailhead tr = trails.trailheads[t];
        if(tr.peaks_count) printf("trail %d {%d,%d} has %d peaks\n",t, tr.pos.x, tr.pos.y, tr.peaks_count);
        else
            continue;

        for (int pks = 0; pks<tr.peaks_count; pks ++)
            printf("{%d,%d} ",tr.peaks_reached[pks].x, tr.peaks_reached[pks].y);

        printf("\n");
        peak_count+=trails.trailheads[t].peaks_count;
    }

    printf("\nPart1 result %d\n", peak_count);
    return 0;
    }