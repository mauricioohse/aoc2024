#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

// #pragma GCC diagnostic ignored "-Wunused-parameter" 
// #pragma GCC diagnostic ignored "-Wwrite-strings"
#define TODO(text) {printf(text); exit(-1);}

#define INPUT "input.txt"
// #define INPUT "test.txt"
#define MAX_SIZE 100
char line[MAX_SIZE] = {0};

int64_t total_cost = 0;

struct Equation_t
{
    int64_t A[2];
    int64_t B[2];
    int64_t C[2];
    int64_t result[2];
} equation;

int64_t sum = 0;

void process_equation(Equation_t equation)
{
    int64_t a1 = equation.A[0];
    int64_t a2 = equation.A[1];
    int64_t b1 = equation.B[0];
    int64_t b2 = equation.B[1];
    int64_t r1 = equation.result[0];
    int64_t r2 = equation.result[1];

    double B_den = a1*r2-a2*r1;
    double B_div = b2*a1-b1*a2;

    double B = B_den/B_div;
    double A = (r1-B*b1)/a1;

    if ( ceil(B)!= B || ceil(A) != A )
        return;

    total_cost += B + A*3;

}

// expects line to end in \0
void process_line(char *line)
{
    char line_type = line[7];

    switch (line_type)
    {
    case 'A':
        sscanf(line, "Button A: X+%lld, Y+%lld", &equation.A[0], &equation.A[1]);
        break;
    case 'B':
        sscanf(line, "Button B: X+%lld, Y+%lld", &equation.B[0], &equation.B[1]);
        break;
    case 'X':
        sscanf(line, "Prize: X=%lld, Y=%lld", &equation.result[0], &equation.result[1]);
        process_equation(equation);
        memset(&equation, 0, sizeof(equation));
        break;

    default:
        break;
    }
}

int main()
{
    memset(&equation,0,sizeof(equation));

    FILE *f = fopen(INPUT, "r");
    int ch, count=0;
    while (1) // this sucks lol
    {
        ch = fgetc(f);
        if (ch != '\0' && ch != '\n' )
        {
            line[count++] = ch;
        }
        else
        {
            process_line(line);
            
            memset(line, 0, sizeof(line));
            count = 0;
        }

        if(ch == EOF)
        {
            process_line(line);
            break;
        }
    }

    printf("result = %llu", total_cost); // 28663, 

    return 0;
}