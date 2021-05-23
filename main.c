#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <stdint.h>
#include <ctype.h>

#define WORDSIZE 100
#define ULP_TOLERANCE 2

typedef union {
    int32_t i;
    float f;
    struct {
        uint32_t mantissa : 23;
        uint32_t exponent : 8;
        uint32_t sign : 1;
    } parts;
} flt_or_int;


typedef struct span {
    char* id;
    flt_or_int min;
    flt_or_int max;
} Span;


void must_alloc(void* ptr, const char* desc){
    if(!ptr){
        printf("Couldn't allocate memory for %s.", desc);
        exit(1);
    }
}


Span* new_span(char* id, float min, float max){
    Span* new = malloc(sizeof(Span));
    must_alloc(new, "new span");

    new->min.f = min;
    new->max.f = max;
    new->id = malloc(sizeof(char) * WORDSIZE);
    must_alloc(new->id, "new span ID");
    strcpy(new->id, id);
    
    return new;
}


int name_to_index(char* name){
    int index = atoi(name + 1);
    return index - 1;
}


Span* add(char* id, Span* A, Span* B){
    float min, max;

    min = A->min.f + B->min.f;
    max = A->max.f + B->max.f;

    if (min != -INFINITY)
        min = nextafterf(min, -FLT_MAX);
    if (max != INFINITY)
        max = nextafterf(max, FLT_MAX);

    return new_span(id, min, max);
}


Span* sub(char* id, Span* A, Span* B){
    float min, max;

    min = A->min.f - B->max.f;
    max = A->max.f - B->min.f;

    if (min != -INFINITY)
        min = nextafterf(min, -FLT_MAX);
    if (max != INFINITY)
        max = nextafterf(max, FLT_MAX);
    
    return new_span(id, min, max);
}


float get_min(float* v, int size){
    float min = v[0];
    for(int i = 1; i < size; i++){
        if(v[i] < min)
            min = v[i];
    }
    return min;
}


float get_max(float* v, int size){
    float max = v[0];
    for(int i = 1; i < size; i++){
        if(v[i] > max)
            max = v[i];
    }
    return max;
}


Span* mult(char* id, Span* A, Span* B){
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

    return new_span(id, min, max);
}


Span* divi(char* id, Span* A, Span* B){
    float min, max;

    if (B->min.f == nextafterf(0.0, FLT_MIN)|| B->max.f == nextafterf(0.0, FLT_MAX)){
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

    return new_span(id, min, max);
}


int almost_equal(flt_or_int A, flt_or_int B){
    if (A.f == -INFINITY || B.f == INFINITY)
        return 0;

    int ulpsDiff = A.i - B.i;
    if (abs(ulpsDiff) <= ULP_TOLERANCE)
        return 1;
    return 0;
}


void magic_print(flt_or_int num){
    printf("f:%1.8e, ix:0x%08X, s:%d, e:%d, mx:0x%06X\n",
            num.f, num.i,
            num.parts.sign, num.parts.exponent, num.parts.mantissa); 
}


void print_results(Span** v, int size, int assignments){
    for(int i = 0; i < size; i++){
        printf("%s = [%20.8e, %20.8e]\n", v[i]->id, v[i]->min.f, v[i]->max.f);
    }


    printf("\nNão unitários:\n");
    for(int i = assignments; i < size; i++){
        if( !almost_equal(v[i]->min, v[i]->max) ){
            printf("%s = [%20.8e, %20.8e]\n", v[i]->id, v[i]->min.f, v[i]->max.f);
        }
    }
}



void main(){
    int assignments;
    int operations;

    fscanf(stdin, "%d %d\n", &assignments, &operations);

    char* line = malloc(sizeof(char) * WORDSIZE);
    must_alloc(line, "line");
    
    int size = assignments + operations;

    Span** values = malloc(sizeof(Span*) * size);
    must_alloc(values, "span array");

    while(fgets(line, WORDSIZE, stdin)){
        char* identifier = strtok(line, " ");
        for(int i = 0; i < strlen(identifier); i++)
            identifier[i] = toupper(identifier[i]);
        
        char* aux = strtok(NULL, " ");

        if(!strcmp(aux, "=")){
            char* operating1 = strtok(NULL, " ");
            char operator = strtok(NULL, " ")[0];
            char* operating2 = strtok(NULL, " ");
            switch (operator){
                case '+':
                    // addition
                    values[name_to_index(identifier)] = add(identifier, values[name_to_index(operating1)], values[name_to_index(operating2)]);
                    break;
                case '-':
                    // subtraction
                    values[name_to_index(identifier)] = sub(identifier, values[name_to_index(operating1)], values[name_to_index(operating2)]);
                    break;
                case '*':
                    // multiplication
                    values[name_to_index(identifier)] = mult(identifier, values[name_to_index(operating1)], values[name_to_index(operating2)]);
                    break;
                case '/': 
                    // division
                    values[name_to_index(identifier)] = divi(identifier, values[name_to_index(operating1)], values[name_to_index(operating2)]);
                    break;
                default:
                    break;
            }
        }
        else {
            flt_or_int num, min, max;
            num.f = atof(aux);
            min.f = nextafterf(num.f, -FLT_MAX);
            max.f = nextafterf(num.f, FLT_MAX);
            values[name_to_index(identifier)] = new_span(identifier, min.f, max.f);
        }
    }

    print_results(values, size, assignments);
}

