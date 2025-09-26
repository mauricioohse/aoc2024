#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

size_t const LINE_SIZE = 200;

int main(){ 

    FILE* f = fopen("test.txt", "r");

    char line[LINE_SIZE] = {0};
    size_t i = 0;

    //memset(equations,0,sizeof(equations));

    while(fgets(line,LINE_SIZE-1,f))
    {
    }

    size_t eq_cnt = i;
    // size_t valid_eq_sum = run_part2(equations, eq_cnt);

    // printf("\nPart2 result %lld\n", valid_eq_sum);

    return 0;
    }