#include <stdio.h>
#include <stdlib.h>

int max(int num1, int num2){
    if(num1 >= num2){
        return  num1;
    }
    else{
        return num2;
    }
}
int main() {
    FILE *f;
    f = fopen("/Users/leiyunsen/Desktop/knapsack1.txt", "r");
    char line[30];
    char *endptr;
    int pack_size, item_num;
    int item_value, item_weight;
    int count = 1;
    fgets(line, 30, f);
    pack_size = (int)strtol(line, &endptr, 10);
    item_num = (int)strtol(endptr, &endptr, 10);
    int value_list[item_num+1];
    int weight_list[item_num+1];
    int knapsack[item_num+1][pack_size+1];
    while (!feof(f)){
        fgets(line, 20, f);
        item_value = (int)strtol(line, &endptr, 10);
        item_weight = (int)strtol(endptr, &endptr, 10);
        value_list[count] = item_value;
        weight_list[count] = item_weight;
        count += 1;
    }
    for (int i = 0; i <=pack_size; ++i) {
        knapsack[0][i] = 0;
    }
    for (int j = 1; j <= item_num; ++j) {
        for (int i = 0; i <= pack_size ; ++i) {
            if(weight_list[j] > i){
                knapsack[j][i] = knapsack[j-1][i];
            }
            else{
                knapsack[j][i] = max(knapsack[j-1][i], knapsack[j-1][i-weight_list[j]]+value_list[j]);
            }
        }
    }
    printf("%d", knapsack[item_num][pack_size]);


}