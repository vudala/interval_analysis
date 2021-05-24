// Eduardo Vudala Senoski GRR20195689

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#include "interval.h"

void capitalize(char* str){
    for(int i = 0; i < strlen(str); i++)
        str[i] = toupper(str[i]);
}

Interval* (*select_op(char operator))(char* id, Interval* A, Interval* B){
    switch (operator){
        case '+': return add;
        case '-': return sub;
        case '*': return mult;
        case '/': return divi;
    }
}


void main(){
    int assignments;
    int operations;

    fscanf(stdin, "%d %d\n", &assignments, &operations);

    int size = assignments + operations;

    Interval** values = malloc(sizeof(Interval*) * size);
    must_alloc(values, "interval array");

    char* line = malloc(sizeof(char) * WORDSIZE);
    must_alloc(line, "line");

    while(fgets(line, WORDSIZE, stdin)){
        char* identifier = strtok(line, " ");
        capitalize(identifier);
        char* aux = strtok(NULL, " ");

        if(!strcmp(aux, "=")){
            char* op1 = strtok(NULL, " ");
            char operator = strtok(NULL, " ")[0];
            char* op2 = strtok(NULL, " ");
            values[name_to_index(identifier)] = select_op(operator)(identifier, values[name_to_index(op1)], values[name_to_index(op2)]);
        }
        else {
            float num = atof(aux);
            float min = nextafterf(num, -FLT_MAX);
            float max = nextafterf(num, FLT_MAX);
            values[name_to_index(identifier)] = new_interval(identifier, min, max);
        }
    }

    print_results(values, size, assignments);
}