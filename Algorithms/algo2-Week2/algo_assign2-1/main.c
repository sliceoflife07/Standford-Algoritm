#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define EDGE_NUM 124750
#define K 4
typedef struct _edge* p2edge;
typedef struct _node* p2node;
struct _edge{
    int node1;
    int node2;
    int edge_cost;
}edge;

struct _node{
    int parent;
    int rank;
}node;

void merge(p2edge edge_list, int start, int mid, int end){
    int k = 0;
    int left_index, right_index;
    int left_size = mid - start +1;
    int right_size = end - mid;
    struct _edge left_arr[left_size+1];
    struct _edge right_arr[right_size+1];
    left_arr[left_size] = (struct _edge){.node1=0, .node2=0, .edge_cost=INT32_MAX};
    right_arr[right_size] = (struct _edge){.node1=0, .node2=0, .edge_cost=INT32_MAX};
    for (int i = start; i <= mid ; ++i) {
        left_arr[k] = edge_list[i];
        k++;
    }
    k = 0;
    for (int j = mid+1; j <=end; ++j) {
        right_arr[k] = edge_list[j];
        k++;
    }
    left_index = right_index = 0;
    for (int l = start; l <=end ; ++l) {
        if(left_arr[left_index].edge_cost <= right_arr[right_index].edge_cost){
            edge_list[l] = left_arr[left_index];
            left_index += 1;
        }
        else{
            edge_list[l] = right_arr[right_index];
            right_index += 1;
        }
    }

}

void merge_sort(p2edge edge_list, int start, int end){
    if(end - start!=0){
        merge_sort(edge_list, start, (start+end)/2);
        merge_sort(edge_list, (start+end)/2+1, end);
        merge(edge_list, start, (start+end)/2, end);
    }
}

int find(p2node node_list, int node_num){
    if(node_list[node_num].parent==node_num){
        return node_num;
    }
    else{
        return find(node_list,node_list[node_num].parent);
    }
}

void setunion(p2node node_list, int node1, int node2){
    int root1, root2;
    int rank1, rank2;

    root1 = find(node_list,node1);
    root2 = find(node_list,node2);
    rank1 = node_list[root1].rank;
    rank2 = node_list[root2].rank;
    if (rank1 > rank2){
        node_list[root2].parent = root1;
    }
    else
        if(rank1 == rank2){
            node_list[root2].parent = root1;
            node_list[root1].rank += 1;
        }
        else{
            node_list[root1].parent = root2;
        }
}

int main() {
    FILE *f;
    char get_line[20];
    int temp_node1, temp_node2, temp_edge_cost;
    int total_node_num;
    int i = 0;
    f = fopen("/Users/leiyunsen/Desktop/clustering1.txt", "r");
    fgets(get_line, 19, f);
    sscanf(get_line,"%d",&total_node_num);
    int cluster_num = total_node_num;
    p2edge edge_list = (p2edge)malloc(sizeof(edge)*EDGE_NUM);
    while (!feof(f)){
        fgets(get_line,19,f);
        sscanf(get_line,"%d %d %d", &temp_node1, &temp_node2, &temp_edge_cost);
        edge_list[i] = (struct _edge){.node1=temp_node1, .node2=temp_node2, .edge_cost=temp_edge_cost};
        i += 1;
    }
    merge_sort(edge_list,0,EDGE_NUM-1);
//    for (int l = 0; l < EDGE_NUM; ++l) {
//        printf("%d %d %d\n",edge_list[l].node1, edge_list[l].node2, edge_list[l].edge_cost);
//    }
    p2node node_list = (p2node)malloc(sizeof(node)*(total_node_num+1));
    for (int j = 0; j <= total_node_num; ++j) {
        node_list[j] = (struct _node){.parent=j, .rank=0};
    }
    i = 0;
    while (cluster_num>=K){
        temp_node1 = edge_list[i].node1;
        temp_node2 = edge_list[i].node2;
//        printf("edge:(%d,%d):%d\n",edge_list[i].node1, edge_list[i].node2, edge_list[i].edge_cost);
//        printf("group:%d %d\n",find(node_list, temp_node1),find(node_list, temp_node2));

        if(find(node_list, temp_node1) != find(node_list, temp_node2)){
            if(cluster_num-1<K){
                break;
            }
            setunion(node_list, temp_node1, temp_node2);
//            printf("merged\n");
            cluster_num -= 1;
        }
        i += 1;
    }
    printf("max spacing:%d\n",edge_list[i].edge_cost);
//    for (int k = 1; k <=total_node_num; ++k) {
//        printf("%d\n",find(node_list, k));
//    }
    return 0;
}