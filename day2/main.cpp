#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))

void print_arr(int *arr, size_t size)
{
    for (size_t i=0; i<size; i++)
        {
            printf("%d ",arr[i]);
        }
}

int is_changing(int a, int b)
{
    if (abs(a-b)>3) return 0; // changed more than three 
    if (a>b) return -1;        // decreasing
    if (a<b) return 1;       // increasing
    return 0;                 // static
}

int row_is_safe(int * row, int row_size, int &first_unsafe_item_idx){

       int last_increasing_state = is_changing(row[0], row[1]);

        for (int j = 0; j < row_size - 1; j++)
        {
            int curr_increasing_state = is_changing(row[j], row[j+1]);

            if (curr_increasing_state == 0 ||
                last_increasing_state != curr_increasing_state)
            {
                first_unsafe_item_idx = j;
                return false;
            }
            last_increasing_state = curr_increasing_state;
        }
    return true;
}


int main(){

    FILE* f = fopen("input.txt", "r");

    printf(" testing\n");
    int reports[1000][50] = {0}; // each row is a report, each column a data point
    int reports_lengths[9999] = {0};
    size_t i = 0;

    char line[50] = {0};
    while (fgets(line,100,f))
    {
        size_t j = 0;
        char * tok = strtok(line," ");
        while(tok!=nullptr){
            reports[i][j] = atoi(tok);
            tok = strtok(NULL," ");
            j++;
        }
        reports_lengths[i] = j;
        i++;
    }

    size_t n_reports = i; 

    // input handled, now logic forpart 1
    size_t valid_counter = 0; // part 1

    size_t tolerated_valid_counter = 0; // part 2

    for (i = 0; i< n_reports; i++)
    {
        int first_unsafe_item_idx = -1;

        if (row_is_safe(reports[i], reports_lengths[i], first_unsafe_item_idx))
        {
            valid_counter++;
            tolerated_valid_counter++;
        }
        else
        {
            // Try removing each item around the unsafe position
            bool can_be_fixed = false;
            for (int remove_idx = MAX(0, first_unsafe_item_idx - 1);
                 remove_idx <= MIN(first_unsafe_item_idx + 1, (int)reports_lengths[i] - 1);
                 remove_idx++)
            {
                // Create new row without the current item
                int new_row[50] = {0};
                int new_idx = 0;
                for (int idx = 0; idx < reports_lengths[i]; idx++) {
                    if (idx != remove_idx) {
                        new_row[new_idx++] = reports[i][idx];
                    }
                }
                
                // Check if this removal fixes the sequence
                int temp_unsafe_idx = -1;
                if (row_is_safe(new_row, reports_lengths[i] - 1, temp_unsafe_idx)) {
                    can_be_fixed = true;
                    break;
                }
            }
            
            if (can_be_fixed) {
                tolerated_valid_counter++;
            }
        }

        //print_arr(reports[i],reports_lengths[i]);
        //printf(" valid_counter %lld, tolerated_valid_counter %lld\n", valid_counter, tolerated_valid_counter);
    }
 
    printf("Part1 result1 %lld\n", valid_counter);
    printf("Part2 result %lld\n", tolerated_valid_counter);

    return 0;
}