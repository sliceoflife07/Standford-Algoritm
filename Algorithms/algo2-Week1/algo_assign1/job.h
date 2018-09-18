#include <stdio.h>
#include <stdlib.h>

typedef int JobLenType;
typedef int JobWgtType;
typedef float JobScrType;
typedef struct Job* PtrToJob;
struct Job{
    JobWgtType job_weight;
    JobLenType job_length;
    JobScrType score;
};

void print_job_list(PtrToJob job_list, int list_length) {
    for (int i = 0; i <list_length ; ++i) {
        printf("%d-%d-%f\n", job_list[i].job_length, job_list[i].job_weight, job_list[i].score);
    }
}

void merge(PtrToJob job_list, int start, int medium, int end) {
    int left_size = medium - start + 1;
    int right_size = end - medium;
    struct Job left_arr[left_size+1];
    struct Job right_arr[right_size+1];
    for (int i = 0; i < left_size; ++i) {
        left_arr[i] = job_list[start+i];
    }
    for (int j = 0; j < right_size; ++j) {
        right_arr[j] = job_list[medium+1+j];
    }
    left_arr[left_size] = right_arr[right_size] = (struct Job){.job_weight=0, .job_length=0, .score=-100000};
    int left_count = 0;
    int right_count = 0;
    for (int k = start; k <=end; ++k) {
        if(left_arr[left_count].score >= right_arr[right_count].score){
            if(left_arr[left_count].score == right_arr[right_count].score){
                if(left_arr[left_count].job_weight >= right_arr[right_count].job_weight){
                    job_list[k] = left_arr[left_count];
                    left_count += 1;
                }
                else{
                    job_list[k] = right_arr[right_count];
                    right_count += 1;
                }
            }
            else{
                job_list[k] = left_arr[left_count];
                left_count += 1;
            }
        }
        else{
            job_list[k] = right_arr[right_count];
            right_count += 1;
        }
    }
}

void merge_sort(PtrToJob job_list, int start, int end) {
    int medium;
    if(start < end){
        medium = (start + end)/2;
        merge_sort(job_list, start, medium);
        merge_sort(job_list, medium+1, end);
        merge(job_list, start, medium, end);
    }
    else{
        return;
    }
}

int job() {
    FILE *fp;
    char temp[30];
    int job_weight = 0;
    int job_length = 0;
    int job_num = 0;
    int count = 0;
    long weighted_sum = 0;
    fp = fopen("/Users/leiyunsen/Desktop/jobs.txt", "r");
    fgets(temp, 30, fp);
    job_num = atoi(temp);
    PtrToJob job_list = (PtrToJob)malloc(sizeof(struct Job)*job_num);
    if(!fp){
        printf("error opening");
        exit(-1);
    }
    int completion_time[job_num];
    while(!feof(fp)){
        fgets(temp, 30, fp);
        sscanf(temp, "%d %d", &job_weight, &job_length);
        job_list[count].job_weight = job_weight;
        job_list[count].job_length = job_length;
        job_list[count].score = (float)job_weight - job_length;
        count += 1;
    }
    merge_sort(job_list, 0, job_num-1);
    completion_time[0] = job_list[0].job_length;
    weighted_sum = completion_time[0] * job_list[0].job_weight;
    for (int i = 1; i < job_num; ++i) {
        completion_time[i]=completion_time[i-1]+job_list[i].job_length;
        weighted_sum += completion_time[i] * job_list[i].job_weight;
    }
    printf("%ld\n",weighted_sum);
    return 0;
}
