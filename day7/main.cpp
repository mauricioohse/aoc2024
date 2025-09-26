#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>


#define LINE_SIZE 200

struct equation_t{
    size_t result;
    long int numbers[20];
    size_t cnt;
}; 

equation_t equations[1000];

void parse_eq(char* line, equation_t *eq){
    char * tok = strtok(line,":");
    eq->result = atoll(tok);
    eq->cnt = 0;

    while((tok=strtok(NULL," ")) != NULL){
        eq->numbers[eq->cnt++] = atol(tok);
    }

}

size_t concat_size_t(size_t a, size_t b){

    size_t temp = b;
    size_t mult = 1;

    while (temp>0){
        temp=temp/10;
        mult*=10;
    }

    return a*mult + b;
}

size_t check_equation_with_mask(equation_t *e, size_t mask){
    size_t curr_mask = mask;

    size_t curr = e->numbers[0];
    for (size_t i = 1 ; i < e->cnt; i++){
        int operation = curr_mask % 3;

        if(operation == 0)
            curr*=e->numbers[i];
        else if(operation == 1)
            curr+=e->numbers[i];
        else
            curr=concat_size_t(curr,e->numbers[i]);

        curr_mask = curr_mask / 3;
    }
    return curr;
}

size_t check_blank_equation(equation_t *e){
    size_t bitmask = 0;

    for (size_t i = 0; i < (size_t)pow(3, e->cnt); i++)
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

    char line[LINE_SIZE] = {0};
    size_t i = 0;

    memset(equations,0,sizeof(equations));

    while(fgets(line,LINE_SIZE-1,f))
    {
        parse_eq(line, &equations[i++]);
    }

    size_t eq_cnt = i;
    size_t valid_eq_sum = run_part2(equations, eq_cnt);

    printf("\nPart2 result %lld\n", valid_eq_sum);

    return 0;
    }