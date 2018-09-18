#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <errno.h>
#include <time.h>
struct HeapStruct;
struct Node;
struct Edge;
typedef int ElementType;
typedef struct HeapStruct *PriorityQueue;
struct Node{
    int position_in_heap;
    int add_flag;
    struct Edge *next;
};
struct Edge{
    int node_num;
    int edge_cost;
    struct Edge *next;
};
struct HeapStruct{
    int capacity;
    int size;
    ElementType* element;
};

PriorityQueue initialize(int element_num){
    PriorityQueue queue = (PriorityQueue)malloc(sizeof(struct HeapStruct));
    queue->element = (ElementType*)malloc(sizeof(ElementType)*(element_num+1));
    queue->capacity = element_num;
    queue->size = 0;
    queue->element[0] = INT32_MIN;
    return queue;
}

int insert(int node_num, ElementType element, PriorityQueue queue, struct Node *node_list, int *position_to_nodenum) {
    int i;
    if(queue->size==queue->capacity){
        perror("The heap is full");
        return -1;
    }
    for(i = ++queue->size; queue->element[i/2] >= element;i /= 2){
        queue->element[i] = queue->element[i/2];
        node_list[position_to_nodenum[i/2]].position_in_heap = i;
        position_to_nodenum[i] = position_to_nodenum[i/2];
    }
    queue->element[i] = element;
    node_list[node_num].position_in_heap = i;
    position_to_nodenum[i] = node_num;
    return i;
}

ElementType delete_min(PriorityQueue queue, struct Node *node_list, int *position_to_nodenum) {
    int i, child;
    ElementType min_element, last_element;
    int node_num_of_last_element;
    if(queue->size==0){
        perror("Heap is empty");
        return 0;
    }
    min_element = queue->element[1];
//    printf("node added:%d\n",position_to_nodenum[1]);
    node_list[position_to_nodenum[1]].position_in_heap = 0;
    node_list[position_to_nodenum[1]].add_flag = 1;
    last_element = queue->element[queue->size];
    node_num_of_last_element = position_to_nodenum[queue->size];
    queue->size -= 1;
    for (i = 1; i * 2 <= queue->size; i = child) {                                     // find smaller child
        child = i * 2;
        if(child != queue->size && queue->element[child] >= queue->element[child+1]){
            child += 1;
        }
        if(last_element >= queue->element[child]){
            queue->element[i] = queue->element[child];                                // smaller child percolate up to the position of its parent
            node_list[position_to_nodenum[child]].position_in_heap = i;              // update the position info of smaller child
            position_to_nodenum[i] = position_to_nodenum[child];                     // update hash table
        }
        else
            break;
    }
    queue->element[i] = last_element;
    node_list[node_num_of_last_element].position_in_heap = i;
    position_to_nodenum[i] = node_num_of_last_element;
    return min_element;
}

int decrease_key(int position, PriorityQueue queue, ElementType new_key, struct Node *node_list, int *position_to_nodenum) {
    int initial_position = position;
    int initial_nodenum = position_to_nodenum[initial_position];
    ElementType temp;
    while (new_key < queue->element[position / 2]) {
        queue->element[position] = queue->element[position / 2];
        node_list[position_to_nodenum[position / 2]].position_in_heap = position;
        position_to_nodenum[position] = position_to_nodenum[position / 2];
        position /= 2;
    }
    queue->element[position] = new_key;
    node_list[initial_nodenum].position_in_heap = position;
    position_to_nodenum[position] = initial_nodenum;
    return position;

}


int main() {
    FILE *fp;
    char temp[100];
    int node_total, edge_total;
    int start_node, end_node, edge_cost;
    int current_add_node_num = 0;
    int current_node;
    int position;
    int mst_weight = 0;
    struct Edge *temp_edge;
    time_t t;
    srand((unsigned) time(&t));
    fp = fopen("/Users/leiyunsen/Desktop/edges.txt", "r");
    fgets(temp, 100, fp);
    sscanf(temp, "%d %d", &node_total, &edge_total);
//    int initial_node = rand()%(node_total-1) +1;
    int initial_node = 500;
    struct Node *node_list = (struct Node*)malloc(sizeof(struct Node)*node_total);
    for (int i = 0; i < node_total; ++i) {
        node_list[i].next = NULL;
        node_list[i].add_flag = 0;
    }
    while (!feof(fp)) {
        fgets(temp, 100, fp);
        sscanf(temp, "%d %d %d", &start_node, &end_node, &edge_cost);

        temp_edge = (struct Edge *) malloc(sizeof(struct Edge));
        temp_edge->node_num = end_node;
        temp_edge->edge_cost = edge_cost;
        temp_edge->next = node_list[start_node].next;
        node_list[start_node].next = temp_edge;

        temp_edge = (struct Edge *) malloc(sizeof(struct Edge));
        temp_edge->node_num = start_node;
        temp_edge->edge_cost = edge_cost;
        temp_edge->next = node_list[end_node].next;
        node_list[end_node].next = temp_edge;


    }
    PriorityQueue queue = initialize(node_total);
    node_list[initial_node].add_flag = 1;
    node_list[initial_node].position_in_heap = 0; // out of the heap;
    int *position_to_nodenum = (int *) malloc(sizeof(int) * node_total);
    for (int j = 0; j < node_total; ++j) {
        position_to_nodenum[j] = 0;
    }
    current_add_node_num += 1;
    current_node = initial_node;

    printf("initial node:%d\n", initial_node);
    while (current_add_node_num < node_total){
        temp_edge = node_list[current_node].next;
        if (temp_edge == NULL){                                                                                          //for node has no outgoing arc
            current_add_node_num += 1;
            current_node = position_to_nodenum[1];
            mst_weight += delete_min(queue, node_list, position_to_nodenum);
            temp_edge = node_list[current_node].next;
        }
        while (temp_edge != NULL) {
            printf("edge:%d %d\n", temp_edge->node_num, temp_edge->edge_cost);
            if (node_list[temp_edge->node_num].add_flag == 0){                                                           //for node has not been added
                if (node_list[temp_edge->node_num].position_in_heap == 0) {                                              //edge not in heap
                    position = insert(temp_edge->node_num, temp_edge->edge_cost, queue, node_list, position_to_nodenum); //insert into heap and update the position
                }
                else {                                                                                                  // edge already in heap
                    position = node_list[temp_edge->node_num].position_in_heap;
                    if (temp_edge->edge_cost < queue->element[position]) {       //  if the edge_cost is smaller than current key
                        position = decrease_key(position, queue, temp_edge->edge_cost, node_list, position_to_nodenum); // update key by decrease_key method and update position
                    }
                }
            }
            temp_edge = temp_edge->next;
        }
        current_add_node_num += 1;
        current_node = position_to_nodenum[1];
        mst_weight += delete_min(queue, node_list, position_to_nodenum);
        printf("node added:%d\n", current_node);
    }
    printf("mst weight = %d\n", mst_weight);
    printf("node num:%d\n", current_add_node_num);
//    for (int k = 1; k <=500; ++k) {
//        printf("%d %d\n", node_list[k].position_in_heap, node_list[k].add_flag);
//    }
    return 0;
}




