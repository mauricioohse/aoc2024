#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define RED_BEGIN "\033[1;31m"
#define RED_END   "\033[0m"

void print_arr(int *arr, size_t size)
{
    for (size_t i=0; i<size; i++)
        {
            printf("%d ",arr[i]);
        }
}



size_t check_if_valid_mult(char* str){
    int value1 = 0, value2 = 0;
    char closing_bracket = 0;
    char identifier[5] = {0};

    if (sscanf(str, "%4s%d,%d%c", identifier, &value1, &value2, &closing_bracket)){
        if ((strcmp("mul(",identifier)==0) && closing_bracket == ')') {
            printf("%.13s ->  %d * %d = ", str, value1, value2);
            return value1*value2;
            }
    }
    printf("%.13s ->  invalid \n", str);
    return 0;
}

char* get_next_dont_pos(char* curr){
    char* ret = strstr(curr,"don't()");
    return ret;
}

char* get_next_do_pos(char* curr){
    char* ret = strstr(curr,"do()");
    return ret;
}


int main(){

    FILE* f = fopen("input.txt", "r");

    char input[9999] = {0}; // each row is a report, each column a data point
    size_t sum = 0;

    while(fgets(input,9999,f))
    {
        char* current = input;
        while ((current = strstr(current, "mul(")) != nullptr) {
            if (current != nullptr) {
                size_t temp_sum = check_if_valid_mult(current);
                sum += temp_sum;
                if (temp_sum > 0) {
                    printf(RED_BEGIN "%lld" RED_END "\n", sum);
                }
            }
            current += 4;
        }
    }

    printf("Part1 result1 %lld\n", sum);
    printf(" ------------- Part2 starting ------------- \n\n\n");

    // part2
    rewind(f);
    size_t enabled_sum = 0;
    bool enabled = true;
    char *dont_pos = nullptr;
    char *do_pos = nullptr;

    while (fgets(input, 9999, f))
    {
        char *current = input;
        dont_pos = get_next_dont_pos(current);
        do_pos = get_next_do_pos(current);

        while ((current = strstr(current, "mul(")) != nullptr)
        {

            if (dont_pos < current && dont_pos != nullptr)
            {
                dont_pos = get_next_dont_pos(current);
                enabled = false;
            }

            if (do_pos < current && do_pos != nullptr)
            {
                do_pos = get_next_do_pos(current);
                enabled = true;
            }

            if (current != nullptr && enabled)
            {
                size_t temp_sum = check_if_valid_mult(current);
                enabled_sum += temp_sum;
                if (temp_sum > 0)
                {
                    printf(RED_BEGIN "%lld" RED_END "\n", enabled_sum);
                }
            }

            current += 4;
        }
    }

    printf("Part2 result %lld\n", enabled_sum);

    return 0;
    }