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

void check_csv() {
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

void update_record(void) {
    char target[128], line[512], buf[64];
    int field = 0;      // 1=position, 2=salary, 3=date
    int updated = 0;

    printf("Exact employee name to update: ");
    if (!fgets(target, sizeof target, stdin)) return;
    chomp(target); 
    if (!*target){ puts("Empty name. Cancelled."); return; }

    for (;;) {
        printf("Update field: 1) Position  2) Salary  3) PaymentDate : ");
        if (!fgets(buf, sizeof buf, stdin)) return;
        if (sscanf(buf, "%d", &field) == 1 && field >= 1 && field <= 3) break;
        puts("Enter 1, 2, or 3.");
    }

    FILE *in  = fopen("employees.csv", "r");
    if (!in){ perror("open employees.csv"); return; }
    FILE *out = fopen("temp.csv", "w");
    if (!out){ perror("open temp.csv"); fclose(in); return; }

    // copy header if present
    if (fgets(line, sizeof line, in)) fputs(line, out);
    else { fclose(in); fclose(out); remove("temp.csv"); puts("File empty."); return; }

    while (fgets(line, sizeof line, in)){
        chomp(line);
        if (!*line) { continue; }

        // split CSV into 4 fields
        char *name = strtok(line, ",");
        char *pos  = strtok(NULL, ",");
        char *sal  = strtok(NULL, ",");
        char *date = strtok(NULL, ",");

        if (!(name && pos && sal && date)) continue;

        // start with existing salary as the default
        double salary_num = strtod(sal, NULL);

        if (strcmp(name, target) == 0) {           // <-- FIXED: match only when equal
        // if (ieq(name, target)) {                // <-- use this instead for case-insensitive
            if (field == 1){
                char newpos[128];
                printf("New position: ");
                if (fgets(newpos, sizeof newpos, stdin)){
                    chomp(newpos); 
                    if (*newpos) strcpy(pos, newpos);
                }
            } else if (field == 2){
                char sline[64];
                printf("New salary: ");
                if (fgets(sline, sizeof sline, stdin)){
                    double v;
                    if (sscanf(sline, "%lf", &v) == 1) salary_num = v;
                    else puts("Invalid number. Keeping old salary.");
                }
            } else { // field == 3
                char newdate[32];
                printf("New date (YYYY-MM-DD): ");
                if (fgets(newdate, sizeof newdate, stdin)){
                    chomp(newdate); 
                    if (*newdate) strcpy(date, newdate);
                }
            }
            updated++;
        }

        // write row back (normalized)
        fprintf(out, "%s,%s,%.2f,%s\n", name, pos, salary_num, date);
    }

    fclose(in); fclose(out);

    if (updated > 0){
        remove("employees.csv");
        rename("temp.csv", "employees.csv");
        printf("✓ Updated %d row(s).\n", updated);
    } else {
        remove("temp.csv");
        puts("Name not found. Nothing updated.");
    }
}

void delete_record(void){
    char target[128], line[512];
    int deleted = 0;

    printf("Exact name to delete: ");
    if (!fgets(target, sizeof target, stdin)) return;
    chomp(target); 
    if (!*target){ puts("Empty name."); return; }

    FILE *in = fopen("employees.csv","r");
    FILE *out = fopen("temp.csv","w");
    if (!in || !out){ perror("open"); if(in)fclose(in); if(out)fclose(out); return; }

    if (fgets(line,sizeof line,in)) fputs(line,out);   // header
    else { fclose(in); fclose(out); remove("temp.csv"); puts("Empty file."); return; }

    while (fgets(line,sizeof line,in)){
        chomp(line);
        if (!*line) continue;

        char *name = strtok(line, ",");
        char *pos  = strtok(NULL, ",");
        char *sal  = strtok(NULL, ",");
        char *date = strtok(NULL, ",");

        if (!(name && pos && sal && date)) continue;

        if (strcmp(name, target) == 0) {   // delete ONLY matching name
            deleted++;
            continue;                      // skip writing -> removed
        }

        // keep everyone else
        fprintf(out, "%s,%s,%s,%s\n", name, pos, sal, date);
    }

    fclose(in); fclose(out);

    if (deleted){
        remove("employees.csv");
        rename("temp.csv","employees.csv");
        printf("✓ Deleted %d row(s) named \"%s\".\n", deleted, target);
    } else {
        remove("temp.csv");
        puts("Name not found. Nothing deleted.");
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