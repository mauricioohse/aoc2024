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

#define LINE_SIZE 200

struct equation_t{
    size_t result;
    long int numbers[20];
    size_t cnt;
}; 

equation_t equations[1000];

void print_eq(equation_t e){
    printf("%llu: ",e.result);
    for (size_t i = 0; i < e.cnt; i++)
        printf("%ld ", e.numbers[i]);

    printf("\n"); 
}

void parse_eq(char* line, equation_t *eq){
    char * tok = strtok(line,":");
    eq->result = atoll(tok);
    eq->cnt = 0;

    while((tok=strtok(NULL," ")) != NULL){
        eq->numbers[eq->cnt++] = atol(tok);
    }

}

size_t check_equation_with_mask(equation_t *e, size_t mask){
    
    size_t curr = e->numbers[0];
    for (size_t i = 1 ; i < e->cnt; i++){
        if(mask & (1 << (i - 1) ))
            curr*=e->numbers[i];
        else
            curr+=e->numbers[i];
    }

    // printf("equation ");
    // print_eq(*e);
    // printf(" mask %llx , result %llu \n", mask, curr);
    return curr;
}

size_t check_blank_equation(equation_t *e){
    size_t bitmask = 0;

    for (size_t i = 0; i < (size_t)pow(2, e->cnt - 1); i++)
    {
        if (check_equation_with_mask(e, bitmask) == e->result){
            return e->result;
        }
        bitmask++;
    }

    return 0;
}

size_t run_part2(equation_t *e, size_t cnt){

    size_t sum = 0;
    for (size_t i=0; i< cnt; i++){
        sum += check_blank_equation(e + i);
    }

    return sum;
}

int main(){

    FILE* f = fopen("input.txt", "r");

    char line[LINE_SIZE-1] = {0};
    size_t i = 0;

    memset(equations,0,sizeof(equations));

    while(fgets(line,LINE_SIZE-1,f))
    {
        parse_eq(line, &equations[i]);
        print_eq(equations[i]);
        i++;
    }

    size_t eq_cnt = i;
    size_t sum_of_valid_equations = run_part2(equations, eq_cnt);

    printf("\nPart2 result %lld\n", sum_of_valid_equations);

    return 0;
}