#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
unsigned int max(unsigned int num1, unsigned int num2){
    if(num1 >= num2){
        return num1;
    }
    else{
        return num2;
    }
}

int main() {
    FILE *f = fopen("/Users/leiyunsen/Desktop/kanpsack_big.txt", "r");
    char line[30];
    char *endptr;
    int pack_size, item_num;
    unsigned int item_value, item_weight;
    int count = 1;
    clock_t start, end;
    double cpu_time_used;
    fgets(line, 30, f);
    pack_size = (int)strtol(line, &endptr, 10);
    item_num = (int)strtol(endptr, &endptr, 10);
    unsigned int value_list[item_num+1];
    unsigned int weight_list[item_num+1];
    unsigned int *knapsack[2];
    for (int i = 0; i < 2; ++i) {
        knapsack[i] = (unsigned int*)malloc(sizeof(unsigned int) * pack_size);
    }
    for (int j = 0; j <=pack_size; ++j) {
        knapsack[0][j] = 0;
    }
    while (!feof(f)){
        fgets(line, 30 ,f);
        item_value = (unsigned int)strtol(line, &endptr, 10);
        item_weight = (unsigned int)strtol(endptr, &endptr, 10);
        value_list[count] = item_value;
        weight_list[count] = item_weight;
        count += 1;
    }
    start = clock();
    for (int i = 1; i <= item_num ; ++i) {
        for (int j = 0; j <= pack_size; ++j) {
            if(weight_list[i]>j){
                knapsack[i&1][j] = knapsack[(i-1) & 1][j];
            }
            else{
                knapsack[i&1][j] = max(knapsack[(i-1)&1][j], knapsack[(i-1)&1][j-weight_list[i]]+value_list[i]);
            }
        }
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("%d\n", knapsack[0][pack_size]);
    printf("%f\n", cpu_time_used);
   return 0;
}