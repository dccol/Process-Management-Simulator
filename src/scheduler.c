
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_CHAR 1000

int main(int argc, char **argv) {

    char str[MAX_CHAR];

    // READ IN THE PROCESSES
    FILE *processes_file = fopen("../test/processes.txt", "r");
    if(processes_file == NULL){
        printf("Could not open file\n");
        return 1;
    }

    // READ BY LINE
    while (fgets(str, MAX_CHAR, processes_file) != NULL) {
        printf("%s", str);

        // SPLIT INTO VARIABLES
        char *token = strtok(str, " ");
        while (token != NULL) {
            printf("%s\n", token);
            token = strtok(NULL, " ");
        }
    }

    fclose(processes_file);
    return 0;



}