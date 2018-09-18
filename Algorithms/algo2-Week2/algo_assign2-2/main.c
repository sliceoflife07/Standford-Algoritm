#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct _node *p2node;
typedef struct _hash_node *p2hash_table;
typedef struct _unionset *p2union;
struct _node {
    unsigned int node_num;
    p2node next;
} node;

struct _hash_node {
    int num;
    p2node next;
} hash_node;

struct _unionset{
    int rank;
    int parent;
}unionset;

int find(p2union union_list, int node_num){
    if(union_list[node_num].parent != node_num){
        union_list[node_num].parent = find(union_list, union_list[node_num].parent);
    }
    return union_list[node_num].parent;
}

void setunion(p2union union_list, int node1, int node2){
    int root1, root2;
    int rank1, rank2;
    root1 = find(union_list, node1);
    root2 = find(union_list, node2);
    rank1 = union_list[node1].rank;
    rank2 = union_list[node2].rank;
    if(rank1==rank2){
        union_list[root2].parent = root1;
        union_list[root1].rank += 1;
    }
    else if(rank1>rank2){
        union_list[root2].parent = root1;
    }
    else{
        union_list[root1].parent = root2;
    }
}

int main() {
    FILE *f;
    char line[50];
    char *endptr;
    int node_total;
    unsigned int vec_num;
    unsigned int node_num;
    int node_count;
    int cluster_count;
    unsigned int hamming1[24] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072,
                        262144, 524288, 1048576, 2097152, 4194304, 8388608};
    unsigned int hamming2[276] = {3, 5, 9, 17, 33, 65, 129, 257, 513, 1025, 2049, 4097, 8193, 16385, 32769, 65537, 131073,
                         262145, 524289, 1048577, 2097153, 4194305, 8388609, 6, 10, 18, 34, 66, 130, 258, 514, 1026,
                         2050, 4098, 8194, 16386, 32770, 65538, 131074, 262146, 524290, 1048578, 2097154, 4194306,
                         8388610, 12, 20, 36, 68, 132, 260, 516, 1028, 2052, 4100, 8196, 16388, 32772, 65540, 131076,
                         262148, 524292, 1048580, 2097156, 4194308, 8388612, 24, 40, 72, 136, 264, 520, 1032, 2056,
                         4104, 8200, 16392, 32776, 65544, 131080, 262152, 524296, 1048584, 2097160, 4194312, 8388616,
                         48, 80, 144, 272, 528, 1040, 2064, 4112, 8208, 16400, 32784, 65552, 131088, 262160, 524304,
                         1048592, 2097168, 4194320, 8388624, 96, 160, 288, 544, 1056, 2080, 4128, 8224, 16416, 32800,
                         65568, 131104, 262176, 524320, 1048608, 2097184, 4194336, 8388640, 192, 320, 576, 1088, 2112,
                         4160, 8256, 16448, 32832, 65600, 131136, 262208, 524352, 1048640, 2097216, 4194368, 8388672,
                         384, 640, 1152, 2176, 4224, 8320, 16512, 32896, 65664, 131200, 262272, 524416, 1048704,
                         2097280, 4194432, 8388736, 768, 1280, 2304, 4352, 8448, 16640, 33024, 65792, 131328, 262400,
                         524544, 1048832, 2097408, 4194560, 8388864, 1536, 2560, 4608, 8704, 16896, 33280, 66048,
                         131584, 262656, 524800, 1049088, 2097664, 4194816, 8389120, 3072, 5120, 9216, 17408, 33792,
                         66560, 132096, 263168, 525312, 1049600, 2098176, 4195328, 8389632, 6144, 10240, 18432, 34816,
                         67584, 133120, 264192, 526336, 1050624, 2099200, 4196352, 8390656, 12288, 20480, 36864, 69632,
                         135168, 266240, 528384, 1052672, 2101248, 4198400, 8392704, 24576, 40960, 73728, 139264,
                         270336, 532480, 1056768, 2105344, 4202496, 8396800, 49152, 81920, 147456, 278528, 540672,
                         1064960, 2113536, 4210688, 8404992, 98304, 163840, 294912, 557056, 1081344, 2129920, 4227072,
                         8421376, 196608, 327680, 589824, 1114112, 2162688, 4259840, 8454144, 393216, 655360, 1179648,
                         2228224, 4325376, 8519680, 786432, 1310720, 2359296, 4456448, 8650752, 1572864, 2621440,
                         4718592, 8912896, 3145728, 5242880, 9437184, 6291456, 10485760, 12582912};
    p2node ptr2node;
    f = fopen("/Users/leiyunsen/Desktop/clustering_big.txt", "r");
    fgets(line, 50, f);
    node_total = (int) strtol(line, &endptr, 10);
    cluster_count = node_total;
    unsigned int node_list[node_total + 1];
    node_count = 1;
    while (!feof(f)) {
        fgets(line, 50, f);
        vec_num = 0;
        for (int i = 0; i < strlen(line); ++i) {
            if (line[i] == 0x31) {
                vec_num <<= 1;
                vec_num ^= 1;
            } else if (line[i] == 0x30) {
                vec_num <<= 1;
                vec_num ^= 0;
            }
        }
        node_list[node_count] = vec_num;
        node_count += 1;
    }
    p2hash_table vec_to_node_hash = (p2hash_table)malloc(sizeof(hash_node) * 16777216);
    p2union union_list = (p2union)malloc(sizeof(unionset)*node_total+1);
    for (int k = 0; k < 16777216; ++k) {
        vec_to_node_hash[k].next = NULL;
        vec_to_node_hash[k].num = 0;
    }
    for (int m = 1; m <=node_total; ++m) {
        union_list[m].rank = 0;
        union_list[m].parent = m;
    }
    for (int j = 1; j <= node_total; ++j) {
        vec_num = node_list[j];
        vec_to_node_hash[vec_num].num += 1;
        ptr2node = vec_to_node_hash[vec_num].next;
        vec_to_node_hash[vec_num].next = (p2node) malloc(sizeof(node));
        vec_to_node_hash[vec_num].next->node_num = (unsigned int) j;
        vec_to_node_hash[vec_num].next->next = ptr2node;
    }

    for (int l = 1; l <=node_total; ++l) {

        vec_num = node_list[l];
        node_count = vec_to_node_hash[vec_num].num;
        while (node_count > 0){
            ptr2node = vec_to_node_hash[vec_num].next;
            node_num = ptr2node->node_num;
            if(find(union_list, l) != find(union_list, node_num)){
                setunion(union_list, l , node_num);
                cluster_count -= 1;
            }
            node_count -= 1;
            ptr2node = ptr2node->next;
        }

        for (int i = 0; i < 24; ++i) {
            vec_num = node_list[l] ^ hamming1[i];
            node_count = vec_to_node_hash[vec_num].num;
            while (node_count > 0){
                ptr2node = vec_to_node_hash[vec_num].next;
                node_num = ptr2node->node_num;
                if(find(union_list, l) != find(union_list, node_num)){
                    setunion(union_list, l, node_num);
                    cluster_count -= 1;
                }
                node_count -= 1;
                ptr2node = ptr2node->next;
            }
        }

        for (int j = 0; j < 276; ++j) {
            vec_num = node_list[l] ^ hamming2[j];
            node_count = vec_to_node_hash[vec_num].num;
            while (node_count > 0){
                ptr2node = vec_to_node_hash[vec_num].next;
                node_num = ptr2node->node_num;
                if(find(union_list, l) != find(union_list, node_num)){
                    setunion(union_list, l, node_num);
                    cluster_count -= 1;
                }
                node_count -=1;
                ptr2node = ptr2node->next;
            }
        }
    }
    printf("cluster:%d\n", cluster_count);
}