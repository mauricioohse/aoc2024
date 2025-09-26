#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <iostream>

#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define RED_BEGIN "\033[1;31m"
#define RED_END   "\033[0m"

using namespace std;
const int SIZE = 150;


void print_arr(char *arr, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        printf("%c", arr[i]);
    }
}


struct page_struct
{
    size_t number;
    size_t previous_pages_numbers[50];
    size_t previous_cnt;
};



const size_t MAX_PAGES = 100;
page_struct g_pages_list[MAX_PAGES];
size_t g_page_list_cnt=0;

struct print_order_t
{
    size_t orders[50];
    size_t n;
};

void print_arr(print_order_t *arr, size_t size)
{
    for (size_t n = 0; n < size; n++)
    {
        for (size_t i = 0; i < arr[n].n; i++)
        {
            if (arr[n].orders[i] != 0)
                printf("%lld,", arr[n].orders[i]);
        }
        printf("\n");
    }
}

void print_arr(page_struct *page){
    printf("page %lld - cnt of previous pages: %lld - Previous pages: ",page->number, page->previous_cnt);
    for (size_t i=0; i< page->previous_cnt; i++)    printf("%lld,",page->previous_pages_numbers[i]);
    printf("\n");
}


void print_arr(page_struct *arr, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        if (arr[i].number != 0)
        {
            print_arr(&arr[i]);
            // printf("page %lld - needs to be after %lld pages:\n", arr[i].number, arr[i].previous_cnt);
        
            // for (size_t j=0; j< arr[i].previous_cnt; j++) printf("%lld,",arr[i].previous_pages_numbers[j]);
        }
    }
}

print_order_t g_printing_orders[300];
size_t g_printing_orders_cnt = 0;

int is_valid_sequent_page_number(page_struct* p, size_t value){

    for (size_t i =0 ; i < p->previous_cnt; i ++)
        if (value == p->previous_pages_numbers[i])
            return 0;

    return 1;
}

size_t get_first_incorrect_order_idx (print_order_t *printing_order, size_t checked_idx)
{
        size_t curr_value = printing_order->orders[checked_idx];
        page_struct *page_data = &g_pages_list[curr_value];

        for (size_t next_values_idx = checked_idx + 1; next_values_idx < printing_order->n; next_values_idx++)
            if (!is_valid_sequent_page_number(page_data, printing_order->orders[next_values_idx]))
                return next_values_idx;

    return 0;
}

size_t sum_if_order_is_valid(print_order_t *printing_order){
    size_t middle_idx = (printing_order->n/2);
    size_t middle_value = printing_order->orders[middle_idx];

    for (size_t i=0; i< printing_order->n - 1;i++){
        // for each value, check if none of the next values are in its previous list
        size_t curr_value = printing_order->orders[i];
        page_struct *page_data = &g_pages_list[curr_value];

        for (size_t next_values_idx = i + 1; next_values_idx < printing_order->n; next_values_idx++ )
            if (!is_valid_sequent_page_number(page_data, printing_order->orders[next_values_idx])) return 0;
    }
    
    return middle_value;
}

bool is_number_sequence_valid(size_t first, size_t second)
{
    page_struct *p = &g_pages_list[first];

    for (size_t i = 0; i < p->previous_cnt; i++)
        if (second == p->previous_pages_numbers[i])
            return false;

    return true;
}

bool is_printing_order_valid(print_order_t *printing_order, size_t &first_wrong_index, size_t &second_wrong_index){

    for (size_t i=0; i< printing_order->n - 1;i++){
        // for each value, check if none of the next values are in its previous list
        size_t curr_value = printing_order->orders[i];

        for (size_t next_values_idx = i + 1; next_values_idx < printing_order->n; next_values_idx++ )
            if (!is_number_sequence_valid(curr_value, printing_order->orders[next_values_idx]))
            {
                first_wrong_index = i;
                second_wrong_index = next_values_idx; 
                return false;
            }
    }
    
    return true;
}

void change_number_order(print_order_t *p , size_t idx_wrong_1, size_t  idx_wrong_2)
{
    if (!idx_wrong_1 && !idx_wrong_2) return;

    size_t wrong_value_1 =  p->orders[idx_wrong_1];

    for (size_t i = idx_wrong_1 ; i < idx_wrong_2; i ++)
    {
            p->orders[i] = p->orders[i + 1];        
    }

    p->orders[idx_wrong_2] =  wrong_value_1;

}

int main(){

    memset(g_printing_orders,0, sizeof(g_printing_orders));
    memset(g_pages_list, 0, sizeof(g_pages_list));

    FILE* f = fopen("input.txt", "r");

    char line[SIZE-1] = {0};
    bool learning_page_order = true;

    while(fgets(line,SIZE-1,f))
    {
        if (learning_page_order){
            // until line = \n, learning page order
            if (line[0] == '\n') {learning_page_order=false; continue;}

            char *tok = strtok(line,"|");
            size_t value_left = atoi(tok);
            tok = strtok(NULL,"|");
            size_t value_right = atoi(tok);

            page_struct *curr_page = &g_pages_list[value_right];

            curr_page->number = value_right;
            curr_page->previous_pages_numbers[curr_page->previous_cnt++] = value_left;
            
            print_arr(curr_page);

            g_page_list_cnt++;
        }
        else 
        {   // learning printing order
            char *tok = strtok(line,",");
            int n = 0;
            g_printing_orders[g_printing_orders_cnt].orders[n++]=atoi(tok);
            while ( (tok = strtok(NULL,",")) != NULL )
            {
                g_printing_orders[g_printing_orders_cnt].orders[n++]=atoi(tok);
            }
            g_printing_orders[g_printing_orders_cnt].n = n;
            g_printing_orders_cnt++;
        }
    }

    printf("printing g_pages_list g_page_list_cnt %lld \n", g_page_list_cnt);
    print_arr(g_pages_list, MAX_PAGES);

    printf("\n now printing orders g_printing_orders_cnt %lld \n", g_printing_orders_cnt);

    print_arr(g_printing_orders, g_printing_orders_cnt);

    // goes through each printing order, check if all order are valid. if they are, sum
    size_t sum = 0;
    size_t sum_reordered = 0;
    for (size_t i=0; i< g_printing_orders_cnt; i ++)
    {
        size_t change = sum_if_order_is_valid(&g_printing_orders[i]);
        if (change != 0)
            printf("sum increased - change %lld - sum %lld \n", change, sum);
        sum += change;
        
        if (change == 0)
        {
            // day 2 logic - printing order is not valid, so we need to change the printing order
            // and sum then
            bool valid_printing_order = 0;

            do
            {
                size_t idx_wrong_1 = 0;
                size_t idx_wrong_2 = 0;
                valid_printing_order = is_printing_order_valid(&g_printing_orders[i],idx_wrong_1, idx_wrong_2);

                if(!valid_printing_order)
                    change_number_order(&g_printing_orders[i], idx_wrong_1, idx_wrong_2);

            } while(!valid_printing_order);

            size_t change_reorded = sum_if_order_is_valid(&g_printing_orders[i]);
            if (change_reorded != 0)
                printf("sum reorded increased - change %lld - sum reorded %lld \n", change_reorded, sum_reordered);
            sum_reordered += change_reorded;

        }
    }

    printf("\nPart1 result1 %lld\n", sum);
    printf(" ------------- Part2 starting ------------- \n\n\n");

    // part2

    printf("Part2 result %lld\n", sum_reordered);

    return 0;
    }