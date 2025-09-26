#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define RED_BEGIN "\033[1;31m"
#define RED_END   "\033[0m"


const int SIZE = 150;
char g_map[SIZE][SIZE] = {0};
size_t g_xmas_cnt = 0;
size_t g_MAS_cnt = 0;
size_t g_row_cnt = 0;


void print_arr(char *arr, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        printf("%c", arr[i]);
    }
}

const int possible_xmas_dirs[][4]={
    {0,0,0,0},
    {0,1,2,3},
    {0,-1,-2,-3}
};

const int possible_MAS_dirs[][3]={
    {-1,0,1},
    {1,0,-1}
};

const char xmas_chars[5]="XMAS";
const char MAS_chars[5]="MAS";

int check_xmas_based_on_dir(size_t starting_x, size_t starting_y, const int dir_x[4], const int dir_y[4])
{
        for (int j = 0; j < 4; j++)
        {
            int curr_x = (int)starting_x + dir_x[j];
            int curr_y = (int)starting_y + dir_y[j];
            if ( curr_x >=0 && curr_y >=0)
            {
                if (g_map[curr_x][curr_y] != xmas_chars[j])
                {
                    return 0;
                }
            }
            else
            {
                return 0;
            }
            
        }

    //printf("valid XMAS at pos (%lld,%lld), dir (%d), (%d) \n",starting_x, starting_y, dir_x[3], dir_y[3]);

    return 1;
}

int check_MAS_based_on_dir(size_t starting_x, size_t starting_y, const int dir_x[3], const int dir_y[3])
{
        for (int j = 0; j < 3; j++)
        {
            int curr_x = (int)starting_x + dir_x[j];
            int curr_y = (int)starting_y + dir_y[j];
            if ( curr_x >=0 && curr_y >=0)
            {
                if (g_map[curr_x][curr_y] != MAS_chars[j])
                {
                    return 0;
                }
            }
            else
            {
                return 0;
            }
            
        }

    printf("valid MAS at pos (%lld,%lld), dir (%d), (%d) \n",starting_x, starting_y, dir_x[2], dir_y[2]);

    return 1;
}

int main(){

    FILE* f = fopen("input.txt", "r");

    char line[SIZE-1] = {0};

    while(fgets(line,SIZE-1,f))
    {
        snprintf(g_map[g_row_cnt],SIZE-1, "%s", line);
        //map[g_row_cnt][SIZE-1]='\n';
        g_row_cnt++;
    }
    print_arr(g_map[0], SIZE*SIZE);


    for(size_t i=0; i<g_row_cnt; i++)
        for(size_t j=0; j<g_row_cnt; j++)
        {

            // part 1: find all XMAS patterns
            for (int k = 0; k < 3; k++)
                for (int i2 = 0; i2 < 3; i2++)
                {
                    g_xmas_cnt += check_xmas_based_on_dir(i, j,
                                                          possible_xmas_dirs[k], possible_xmas_dirs[i2]);
                }

            int temp = 0;
            // part 2: find all MAS in X pattern
            for (int k = 0; k < 2; k++)
                for (int i2 = 0; i2 < 2; i2++)
                {
                    temp += check_MAS_based_on_dir(i, j,
                                                          possible_MAS_dirs[k], possible_MAS_dirs[i2]);
                }

        if (temp==2){ g_MAS_cnt++; printf("    | -------------- g_MAS_cnt %lld\n", g_MAS_cnt);}
        
        }


    printf("\nPart1 result1 %lld\n", g_xmas_cnt);
    printf(" ------------- Part2 starting ------------- \n\n\n");

    // part2
    rewind(f);

    printf("Part2 result %lld\n", g_MAS_cnt);

    return 0;
    }