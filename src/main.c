#include <stdio.h>
#include <stdlib.h>
#include "./second_main.c"

// Version 0.1a

void display_data(), create_user();

int main() {
    int choice;

    while (1) {
       
        display_data();
    
        printf("Choice: ");
        scanf("%d", &choice);
        printf("\n");

        menu_check(choice);
    
    }
    return 0;
}

void create_user() {
    
    char name[50];
    printf("Input Name: ");
    scanf("%s", name);

    FILE* file = fopen("data.csv", "w");

    if (file == NULL) {
        printf("File Failed: %p\n", file);
    } 

    fprintf(file, "The owner of this file is: %s \n", name);

    if (fclose(file) == 0) {
        printf("Close the file\n");
    } else {
        printf("Cannot close the file\n");
    }
}