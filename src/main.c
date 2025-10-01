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



void chomp(char *c) {
    c[strcspn(c, "\r\n")] = 0;
}

void clear_line() {
    int c = getchar();
    if (c == EOF) return;
    if (c != '\n') {
        // there was extra stuff on the line; put back the first char, then drain
        ungetc(c, stdin);
        while ((c = getchar()) != '\n' && c != EOF) {}
    }
}


void check_csv() {\
    char ans[64];
    int choice;

    FILE *file = fopen("employees.csv", "r");

    if (!file) {
        perror("Couldn't open the file or not found.\n");

        printf("Do you wish to create new employees file\n");
        printf("1) Yes 2) No \n");
        printf("Input: ");

        if (!fgets(ans, sizeof ans, stdin)) { return; }
        chomp(ans);

         if ((sscanf(ans, "%d", &choice) != 1) || (choice > 7 || choice < 1)) {
            printf("\n\n-------- WARNING ---------------------------\n");
            printf(" \033[33m Invalid input lil bro 🥀 \033[0m         \n");
            printf("--------------------------------------------\n\n");
            return;
         }
        
        if (choice == 1) {
            FILE *newFile = fopen("employees.csv", "w");

            if (!newFile) {
                perror("Couldn't open the file or not found.\n");
                return;
            }

            fputs("EmployeeName,Position,Salary,PaymentDate\n", newFile);
            //fprintf(newFile, "EmployeeName,Position,Salary,PaymentDate\n");
            fclose(newFile);
        } else if (choice == 2) {
            puts("K bro, find employees.csv then.");
        } 

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
    printf("   \e[1m Employee Payroll System 🏦 \e[0m                                \n");
    printf("===============================================================         \n");
    printf("1) List all record                                                      \n");
    printf("2) Create salary record                                                 \n");
    printf("3) Search salary record                                                 \n");
    printf("4) Update salary record                                                 \n");
    printf("5) Delete salary record                                                 \n");
    printf("6) Unit test [2]                                                        \n");
    printf("7) Exit                                                                 \n");
    printf("===============================================================         \n");
}

void create_record() {

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

void search_record() {
    char line[512], buffer[32], query[128];
    int found = 0; 
    int mode; // Have two modes, 1) Name, 2) Position 
    
    // Mode Query
    do
    {
        printf("Search by (1) Name (2) Position, (1-2): "); 
        fgets(buffer, sizeof buffer, stdin);
    } while ((sscanf(buffer, "%d", &mode) != 1) && (mode == 1 || mode == 2));
    
    switch (mode)
    {
    case 1:
        printf("Input Name (eg. John Doe): ");
        fgets(query, sizeof query, stdin);
        break;
    case 2:
        printf("Input Position (Developer): ");
        fgets(query, sizeof query, stdin);
        break;
    default:
        printf("Invalid Input\n");
        break;
    }
    chomp(query);

    FILE *file = fopen("employees.csv", "r");
    if (!file) { perror("Couldn't Open the File!\n"); return;}

    // Read the first line of file
    if (fgets(line, sizeof line ,file) == NULL) {
        printf("File Empty.\n");
        return;
    }

    printf("\n-----------------------------------------------------------------------------\n");
    printf("%-24s %-24s %-14s %-12s\n", "EmployeeName", "Position", "Salary", "PaymentData");
    printf("-----------------------------------------------------------------------------\n");

    while (fgets(line, sizeof line, file)) {
        chomp(line);
        if (!*line) { continue;} // Skip blank string
        
        char *name = strtok(line, ",");
        char *position = strtok(NULL, ",");
        char *salary = strtok(NULL, ",");
        char *paymentdata = strtok(NULL, ",");

        switch (mode)
        {
        case 1: // Name
            if (strcmp(name, query) == 0) {
                printf("%-24s %-24s %-14s %-12s\n", name, position, salary, paymentdata);
                ++found;
            }
            break;
        case 2: // Position
            if (strcmp(position, query) == 0) {
                printf("%-24s %-24s %-14s %-12s\n", name, position, salary, paymentdata);
                ++found;
            }
            break;
        default:
            printf("Error\n");
            break;
        }
    }
    if (!found) {
        printf("!\n");
    }
    printf("-----------------------------------------------------------------------------\n\n");
}

void update_record() {
    FILE *file = fopen("employees.csv", "r");
    FILE *temp = fopen("temp.csv", "w");

    if (!file || !temp) {
        perror("Couldn't open the file or not found.\n");
        return;
    }


}

void delete_record() {

    char target[128];
    char line[512];
    int deleted_count = 0;
    
    printf("Exact name to delete (eg. Walt Heisenberg): ");
    if (!fgets(target ,sizeof target,stdin)) { return; }
    chomp(target);
    if (!*target) { puts("Empty Name, Cancelled.\n"); return; }

    FILE *file = fopen("employees.csv", "r");
    FILE *temp = fopen("temp.csv", "w");

    if (!file || !temp) {
        perror("Couldn't open the file or not found.\n");
        return;
    }

    // Copy the header of original file to the temporary file
    if (fgets(line ,sizeof line, file)) {
        fputs(line, temp);
    } else {
        fclose(file); fclose(temp);
        remove("temp.csv");
        puts("The File is Empty, Nothing to delete bozo\n");
        return;
    }

    while (fgets(line, sizeof line, file)) {
        chomp(line);
        if (!*line) { continue; } // Skip the blank line

        char *name = strtok(line, ",");
        char *pos  = strtok(NULL, ",");
        char *sal  = strtok(NULL, ",");
        char *date = strtok(NULL, ",");

        if (strcmp(name, target)) {
            ++deleted_count;
            continue; // Skip the target's line to remove
        }

        fprintf(temp, "%s,%s,%s,%s\n", name, pos, sal, date);
    }

    fclose(temp); fclose(file);

    if (deleted_count > 0) {
        remove("employees.csv");
        rename("temp.csv", "employees.csv");

        if (deleted_count == 1)
            printf("✓ Deleted \"%s\".\n", target);
        else
            printf("✓ Deleted %d rows named \"%s\".\n", deleted_count, target);
    } else {
        puts("Nothing to Delete\n");
        remove("temp.csv");
    }
}

int main() {
    char input_choice[64];
    int choice;
    
    check_csv();

    while (1) {
        display_data();
        printf("Enter your choice (1-7): ");
        fgets(input_choice, sizeof input_choice ,stdin);
        chomp(input_choice);
    
        if ((sscanf(input_choice, "%d", &choice) != 1) || (choice > 7 || choice < 1)) {
            printf("\n\n-------- WARNING ---------------------------\n");
            printf(" \033[33m Invalid input lil bro 🥀 \033[0m         \n");
            printf("--------------------------------------------\n\n");
            continue;
        }
        
        /*
        scanf("%d", &choice);
        clear_line(); //Buffer
        */
       
        switch (choice)
        {
        case 1: 
            list_all(); 
            break;
        case 2: 
            create_record(); 
            break;
        case 3: 
            search_record(); 
            break;
        case 4:
            update_record(); 
            break;
        case 5: 
            delete_record(); 
            break;
        case 6: 
            //Unit Test
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