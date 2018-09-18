#include <stdio.h>
#include <stdlib.h>

long max(long num1, long num2){
    if(num1 >= num2){
        return num1;
    }
    else{
        return  num2;
    }
}

int main() {
    FILE *f;
    char line[20];
    char *endptr;
    int vertex_num;
    int count = 1;
    long vertex_weight;
    f = fopen("/Users/leiyunsen/Desktop/mwis.txt", "r");
    fgets(line, 20 ,f);
    vertex_num = (int)strtol(line, &endptr, 10);
    long vertex_list[vertex_num+1];
    long IS_value[vertex_num+1];
    int IS_vertex[vertex_num+1];
    for (int j = 0; j <=vertex_num; ++j) {
        IS_vertex[j] = 0;
    }
    IS_value[0] = 0;
    IS_value[1] = vertex_list[1];
    while(!feof(f)){
        fgets(line, 20, f);
        vertex_weight = strtol(line, &endptr, 10);
        vertex_list[count++] = vertex_weight;
    }
    for (int i = 2; i <=vertex_num; ++i) {
        IS_value[i] = max(IS_value[i-1], IS_value[i-2] + vertex_list[i]);
    }

    count = vertex_num;
    while (count >= 1){
        if(IS_value[count-1] >=IS_value[count-2]+vertex_list[count]){
            count -= 1;
        }
        else{
                IS_vertex[count] = 1;
                count -= 2;
        }
    }

    for (int i = 1; i <=1000 ; ++i) {
        if(i==1 || i==2 || i==3 || i==4 || i==17 || i==117 || i==517 || i==997){
            printf("%d",IS_vertex[i]);
        }
    }

    return 0;
}