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
            float num = atof(aux);
            float min = nextafterf(num, -FLT_MAX);
            float max = nextafterf(num, FLT_MAX);
            values[name_to_index(identifier)] = new_interval(identifier, min, max);
        }
    }

    print_results(values, size, assignments);
}