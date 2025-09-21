#include <stdio.h>

int menu_check(int choice) {
    switch (choice)
        {
        case 1:
            printf("New user data\n");
            //create_user();
            break;
        case 2:
            printf("Search user data\n");
            break;
        case 3:
            printf("Edit user data\n");
            break;
        case 4:
            printf("Delete user data\n");
            break;
        case 5:
            return 0;
            break;
        default:
            perror("Invalid Input!\n");
            return -1;
            break;
        }
}

void display_data() {
    printf("\n========================= \n");
    printf("1. New user data \n");
    printf("2. Search user data \n");
    printf("3. Edit user data \n");
    printf("4. Delete user data \n");
    printf("5. Exit the program \n");
    printf("========================== \n");
}

int test_function() {
    return 3;
}
