#ifndef INTERVAL_INCLUDED
#define INTERVAL_INCLUDED

#include <stdint.h>
#include <float.h>

#define WORDSIZE 100
#define ULP_TOLERANCE 2

typedef union {
    int32_t i;
    float f;
} flt_or_int;


typedef struct interval {
    char* id;
    flt_or_int min;
    flt_or_int max;
} Interval;


void must_alloc(void* ptr, const char* desc);

Interval* new_interval(char* id, float min, float max);

int name_to_index(char* name);

Interval* add(char* id, Interval* A, Interval* B);
Interval* sub(char* id, Interval* A, Interval* B);
Interval* mult(char* id, Interval* A, Interval* B);
Interval* divi(char* id, Interval* A, Interval* B);

void print_results(Interval** v, int size, int assignments);

#endif