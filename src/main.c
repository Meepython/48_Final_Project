#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
    CSV Structures
    EmployeeName, Position, Salary, PaymentData
    -------------------------------------------
    John Doe, Manager, 50000, 2025-08-01
    Jane Smith, Developer, 40000, 2025-08-05
*/

void clear_line() {
    int c = getchar();
    if (c == EOF) return;
    if (c != '\n') {
        // there was extra stuff on the line; put back the first char, then drain
        ungetc(c, stdin);
        while ((c = getchar()) != '\n' && c != EOF) {}
    }
}

void chomp(char *c) {
    c[strcspn(c, "\n")] = 0;
}

void open_file(int mode) {

    switch (mode) 
    {
    case 1:
        
        break;
    case 2:
        /*  */
        break;
    default:
        break;
    }

}

void list_all() {
    char line[1000];
    char *EmployeeName;
    char *Position;
    char *Salary;
    char *PaymentData;

    FILE *file = fopen("employees.csv", "r");

    if (file == NULL) {
        perror("Couldn't open the file or not found.\n");
        return;
    }

    // Read the first line of CSV file 
    if (!fgets(line, sizeof line, file)) {
        printf("File Empty.\n");
        return;
    } 
              
    printf("\n-----------------------------------------------------------------------------\n");
    printf("%-24s %-24s %-14s %-12s\n", "EmployeeName", "Position", "Salary", "PaymentData");
    printf("-----------------------------------------------------------------------------\n");

    while (fgets(line, sizeof line, file)) {
        chomp(line);

        char *tempName; 
        char *tempPosition;
        char *tempSalary;
        char *tempPaymentData;

        tempName = strtok(line, ",");
        tempPosition = strtok(NULL, ",");
        tempSalary = strtok(NULL, ",");
        tempPaymentData = strtok(NULL, ",");

        printf("%-24s %-24s %-14s %-12s\n", tempName, tempPosition, tempSalary, tempPaymentData);
       
    }

    printf("-----------------------------------------------------------------------------\n\n");

    if (fclose(file) != 0) {
        perror("Couldn't Close the file");
    }
}

void display_data() {
    printf("===============================================================         \n");
    printf("   \e[1m Employee Payroll System 🏦 \e[0m            \n");
    printf("===============================================================         \n");
    printf("1) List all record                                   \n");
    printf("2) Create salary record                              \n");
    printf("3) Search salary record                              \n");
    printf("4) Update salary record                              \n");
    printf("5) Delete salary record                              \n");
    printf("6) Unit test [2]                                     \n");
    printf("7) Exit                                              \n");
    printf("===============================================================         \n");
}

void create_user() {

    // Open CSV file in append mode
    FILE *file = fopen("employees.csv", "a");

    if (file == NULL) {
        perror("Couldn't open the file or not found.\n");
        return;
    } 

    char new_name[128];
    char new_position[100];
    char salary_input[100];
    float new_salary;
    char payment_date[100];
    
    clear_line();
    // New Employee's Name
    printf("New name (Name Surname): ");
    fgets(new_name, sizeof new_name, stdin);
    chomp(new_name);
    
    // New Employee's Position
    printf("Position: ");
    fgets(new_position, sizeof new_position, stdin);
    chomp(new_position);

    // New Employee's Salary
    printf("Salary: ");
    fgets(salary_input, sizeof salary_input, stdin);
    chomp(salary_input);
    
    if (sscanf(salary_input, "%f", &new_salary) != 1) {
        printf("\n-------- WARNING ---------------------------\n");
        printf(" \033[33m Invalid input lil bro 🥀 \033[0m         \n");
        printf("--------------------------------------------\n\n");
        return;
    }

    // New Employee's Payment Data
    printf("Payment Date (YYYY-MM-DD): ");
    fgets(payment_date, sizeof payment_date, stdin);
    chomp(payment_date);

    fprintf(file, "%s,%s,%.2f,%s\n", new_name, new_position, new_salary, payment_date);

    // Close the file
    if (fclose(file) == 0) {
        printf("\n--------------------------------------------\n");
        printf("\033[32m New Data Succesfully Created.\033[0m \n");
        printf("--------------------------------------------\n\n");
    } else {
        printf("Cannot close the file bruh\n");
    }
}

void search_user() {

}

void update_user() {
    FILE *file = fopen("employees.csv", "r");
    FILE *temp = fopen("temp.csv", "w");

    if (!file || !temp) {
        perror("Couldn't open the file or not found.\n");
        return;
    }


}

void delete_user() {
    FILE *file = fopen("employees.csv", "r");
    FILE *temp = fopen("temp.csv", "w");

    if (!file || !temp) {
        perror("Couldn't open the file or not found.\n");
        return;
    }
}

int main() {
    char input_choice[64];
    int choice;
    while (1) {
        display_data();
        printf("Enter your choice: ");
       
        if(!fgets(input_choice, sizeof input_choice ,stdin)) {
            continue; // EOF
        }

        chomp(input_choice);
    
        if (sscanf(input_choice, "%d", &choice) != 1) {
            printf("\n\n-------- WARNING ---------------------------\n");
            printf(" \033[33m Invalid input lil bro 🥀 \033[0m         \n");
            printf("--------------------------------------------\n\n");
            continue;
        }

        if (choice > 7 || choice < 1) {
            printf("\n\n-------- WARNING ---------------------------\n");
            printf(" \033[33m Choose 1-7 lil bro 🥀 \033[0m         \n");
            printf("--------------------------------------------\n\n");
            continue;
        }

        switch (choice)
        {
        case 1: 
            list_all(); 
            break;
        case 2: 
            create_user(); 
            break;
        case 3: 
            //search_user(); 
            break;
        case 4:
            //update_user(); 
            break;
        case 5: 
            /*Delete*/ 
            break;
        case 6: 
            /*Unit Test*/ 
            //open_file(1);
            break;
        case 7: 
            return 0; 
            break;
        default: 
            printf("Invalid Input!\n");  
        }
    
    }
    return 0;
}