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

int main()
{

    FILE *f = fopen("input.txt", "r");

    char line[LINE_SIZE] = {0};
    size_t i = 0;

    memset(equations, 0, sizeof(equations));

    while (fgets(line, LINE_SIZE - 1, f))
    {
        parse_eq(line, &equations[i++]);
    }

    size_t eq_cnt = i;
    size_t valid_eq_sum = run_part2(equations, eq_cnt);

    printf("\nPart2 result %lld\n", valid_eq_sum);

    return 0;
}

/* 
``` question: part 1 and 2

--- Day 7: Bridge Repair ---
The Historians take you to a familiar rope bridge over a river in the middle of a jungle. The Chief isn't on this side of the bridge, though; maybe he's on the other side?

When you go to cross the bridge, you notice a group of engineers trying to repair it. (Apparently, it breaks pretty frequently.) You won't be able to cross until it's fixed.

You ask how long it'll take; the engineers tell you that it only needs final calibrations, but some young elephants were playing nearby and stole all the operators from their calibration equations! They could finish the calibrations if only someone could determine which test values could possibly be produced by placing any combination of operators into their calibration equations (your puzzle input).

For example:

190: 10 19
3267: 81 40 27
83: 17 5
156: 15 6
7290: 6 8 6 15
161011: 16 10 13
192: 17 8 14
21037: 9 7 18 13
292: 11 6 16 20
Each line represents a single equation. The test value appears before the colon on each line; it is your job to determine whether the remaining numbers can be combined with operators to produce the test value.

Operators are always evaluated left-to-right, not according to precedence rules. Furthermore, numbers in the equations cannot be rearranged. Glancing into the jungle, you can see elephants holding two different types of operators: add (+) and multiply (*).

Only three of the above equations can be made true by inserting operators:

190: 10 19 has only one position that accepts an operator: between 10 and 19. Choosing + would give 29, but choosing * would give the test value (10 * 19 = 190).
3267: 81 40 27 has two positions for operators. Of the four possible configurations of the operators, two cause the right side to match the test value: 81 + 40 * 27 and 81 * 40 + 27 both equal 3267 (when evaluated left-to-right)!
292: 11 6 16 20 can be solved in exactly one way: 11 + 6 * 16 + 20.
The engineers just need the total calibration result, which is the sum of the test values from just the equations that could possibly be true. In the above example, the sum of the test values for the three equations listed above is 3749.

Determine which equations could possibly be true. What is their total calibration result?

Your puzzle answer was 5837374519342.

The first half of this puzzle is complete! It provides one gold star: *

--- Part Two ---
The engineers seem concerned; the total calibration result you gave them is nowhere close to being within safety tolerances. Just then, you spot your mistake: some well-hidden elephants are holding a third type of operator.

The concatenation operator (||) combines the digits from its left and right inputs into a single number. For example, 12 || 345 would become 12345. All operators are still evaluated left-to-right.

Now, apart from the three equations that could be made true using only addition and multiplication, the above example has three more equations that can be made true by inserting operators:

156: 15 6 can be made true through a single concatenation: 15 || 6 = 156.
7290: 6 8 6 15 can be made true using 6 * 8 || 6 * 15.
192: 17 8 14 can be made true using 17 || 8 + 14.
Adding up all six test values (the three that could be made before using only + and * plus the new three that can now be made by also using ||) produces the new total calibration result of 11387.

Using your new knowledge of elephant hiding spots, determine which equations could possibly be true. What is their total calibration result?

Answer: 
 
```
*/