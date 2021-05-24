#include "interval.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void must_alloc(void* ptr, const char* desc){
    if(!ptr){
        printf("Couldn't allocate memory for %s.", desc);
        exit(1);
    }
}


int is_smaller(float a, float b){
    float min = fminf(a, b);
    if (min == a)
        return 1;
    return 0;
}


int is_greater(float a, float b){
    float max = fmaxf(a, b);
    if (max == a)
        return 1;
    return 0;
}


void check_for_errors(Interval* inter){
    if (is_greater(inter->min.f, inter->max.f)) exit(-1);
    if (inter->min.f == NAN || inter->max.f == NAN) exit(-1);
}


Interval* new_interval(char* id, float min, float max){
    Interval* new = malloc(sizeof(Interval));
    must_alloc(new, "new interval");

    new->min.f = min;
    new->max.f = max;
    new->id = malloc(sizeof(char) * WORDSIZE);
    must_alloc(new->id, "new interval ID");
    strcpy(new->id, id);
    
    check_for_errors(new);

    return new;
}


int name_to_index(char* name){
    int index = atoi(name + 1);
    return index - 1;
}


Interval* add(char* id, Interval* A, Interval* B){
    float min, max;

    min = A->min.f + B->min.f;
    max = A->max.f + B->max.f;

    if (min != -INFINITY)
        min = nextafterf(min, -FLT_MAX);
    if (max != INFINITY)
        max = nextafterf(max, FLT_MAX);

    return new_interval(id, min, max);
}


Interval* sub(char* id, Interval* A, Interval* B){
    float min, max;

    min = A->min.f - B->max.f;
    max = A->max.f - B->min.f;

    if (min != -INFINITY) 
        min = nextafterf(min, -FLT_MAX);
    if (max != INFINITY)
        max = nextafterf(max, FLT_MAX);
    
    return new_interval(id, min, max);
}


float get_min(float* v, int size){
    float min = v[0];
    for(int i = 1; i < size; i++)
        if(is_smaller(v[i], min))
            min = v[i];
    return min;
}


float get_max(float* v, int size){
    float max = v[0];
    for(int i = 1; i < size; i++)
        if(is_greater(v[i], max))
            max = v[i];
    return max;
}


Interval* mult(char* id, Interval* A, Interval* B){
    float nums[4];

    nums[0] = A->min.f * B->min.f;
    nums[1] = A->min.f * B->max.f;
    nums[2] = A->max.f * B->min.f;
    nums[3] = A->max.f * B->max.f;

    float min = get_min(nums, 4);
    float max = get_max(nums, 4);

    if (min != -INFINITY)
        min = nextafterf(min, -FLT_MAX);
    if (max != INFINITY)
        max = nextafterf(max, FLT_MAX);

    return new_interval(id, min, max);
}


Interval* divi(char* id, Interval* A, Interval* B){
    float min, max;

    if (B->min.f == nextafterf(0.0, -FLT_MAX) || B->max.f == nextafterf(0.0, FLT_MAX)){
        min = -INFINITY;
        max = INFINITY;
    }
    else {
        float nums[4];

        nums[0] = A->min.f / B->max.f;
        nums[1] = A->min.f / B->min.f;
        nums[2] = A->max.f / B->max.f;
        nums[3] = A->max.f / B->min.f;

        min = get_min(nums, 4);
        max = get_max(nums, 4);

        if (min != -INFINITY)
            min = nextafterf(min, -FLT_MAX);
        if (max != INFINITY)
            max = nextafterf(max, FLT_MAX);
    }

    return new_interval(id, min, max);
}


int almost_equal(flt_or_int A, flt_or_int B){
    if (A.f == -INFINITY || B.f == INFINITY)
        return 0;

    int ulp = A.i - B.i;

    if (abs(ulp) <= ULP_TOLERANCE)
        return 1;
    return 0;
}


void print_results(Interval** v, int size, int assignments){
    for(int i = 0; i < size; i++)
        printf("%s = [%20.8e, %20.8e]\n", v[i]->id, v[i]->min.f, v[i]->max.f);

    printf("\nNão unitários:\n");
    for(int i = assignments; i < size; i++){
        if( !almost_equal(v[i]->min, v[i]->max) ){
            printf("%s = [%20.8e, %20.8e]\n", v[i]->id, v[i]->min.f, v[i]->max.f);
        }
    }
}