#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct _char_node *ptr2node;
typedef struct _heap *ptr2heap;
struct _char_node {
    long frequency;
    int min_length;
    int max_length;

};

struct _heap {
    int capacity;
    int size;
    ptr2node node_array;

};

ptr2heap initialize_heap(int max_element_num) {
    ptr2heap p_heap;
    p_heap = (ptr2heap) malloc(sizeof(struct _heap));
    if (p_heap == NULL) {
        perror("Out of Space");
        exit(-1);
    }
    p_heap->node_array = (ptr2node) malloc(sizeof(struct _char_node) * (max_element_num + 1));
    if (p_heap->node_array == NULL) {
        perror("Out of Space");
        exit(-1);
    }
    p_heap->capacity = max_element_num;
    p_heap->size = 0;
    p_heap->node_array[0] = (struct _char_node) {.frequency=LONG_MIN, .min_length=0, .max_length=0};

    return p_heap;
}

void insert(ptr2heap p_heap, long frequency, int min_length, int max_length){
    int i;
    if (p_heap->size == p_heap->capacity) {
        perror("queue is full");
        exit(-1);
    }
    for (i = ++p_heap->size; p_heap->node_array[i/2].frequency > frequency; i /= 2) {
        p_heap->node_array[i] = p_heap->node_array[i/2];

    }
    p_heap->node_array[i].frequency = frequency;
    p_heap->node_array[i].min_length = min_length;
    p_heap->node_array[i].max_length = max_length;

}

struct _char_node delete_min(ptr2heap p_heap){
    int i, child;
    struct _char_node min_node;
    struct _char_node last_node;
    min_node = p_heap->node_array[1];
    last_node = p_heap->node_array[p_heap->size];
    p_heap->size -= 1;
    for (i = 1; i * 2 <= p_heap->size; i = child) {
        child = i * 2;
        if(child != p_heap->size && p_heap->node_array[child].frequency > p_heap->node_array[child+1].frequency){
            child += 1;
        }
        if(last_node.frequency > p_heap->node_array[child].frequency){
            p_heap->node_array[i] = p_heap->node_array[child];
        }
    }
    p_heap->node_array[i] = last_node;
    return min_node;
}

int max(int num1, int num2){
    if (num1 > num2){
        return num1;
    }
    else{
        return num2;
    }
}

int min(int num1, int num2){
    if(num1 < num2){
        return num1;
    }
    else{
        return num2;
    }

}



int main() {
    FILE *f;
    int symbol_total_num;
    long temp_frequency;
    long frequency_sum = 0;
    ptr2heap p_heap;
    char line[20];
    char *endptr;
    struct _char_node temp_node1, temp_node2, temp_meta_node;
    f = fopen("/Users/leiyunsen/Desktop/huffman.txt", "r");
    fgets(line, 20, f);
    symbol_total_num = (int)strtol(line, &endptr, 10);
    p_heap = initialize_heap(symbol_total_num);

    while (!feof(f)){
        fgets(line, 20 , f);
        temp_frequency = strtol(line, &endptr, 10);
        insert(p_heap, temp_frequency, 0, 0);
    }


    while(p_heap->size>=1) {
        temp_node1 = delete_min(p_heap);
        temp_node2 = delete_min(p_heap);

        temp_meta_node.frequency = temp_node1.frequency + temp_node2.frequency;
        temp_meta_node.min_length = min(temp_node1.min_length, temp_node2.min_length) + 1;
        temp_meta_node.max_length = max(temp_node1.max_length, temp_node2.max_length) + 1;
        insert(p_heap, temp_meta_node.frequency, temp_meta_node.min_length, temp_meta_node.max_length);
    }
    printf("min length:%d\n", p_heap->node_array[p_heap->size].min_length);
    printf("max length:%d\n", p_heap->node_array[p_heap->size].max_length);

    return 0;
}