#include <stdio.h>

// Version 0.1

int main() {
    int answer;

    while (1) {
        scanf("%d", &answer);

        switch (answer)
        {
        case 1:
            printf("New user\n");
            break;
        case 2:
            printf("Search user\n");
            break;
        case 3:
            printf("Edit user\n");
            break;
        case 4:
            printf("Delete user\n");
            break;
        default:
            printf("Invalid Input!\n");
            break;
        }
    }

    return 0;
}

void display_data() {
    printf("========================= \n");
    printf("1. New user data \n");
    printf("2. Search user data \n");
    printf("3. Edit user data \n");
    printf("4. Delete user data \n");
    printf("========================== \n");
}