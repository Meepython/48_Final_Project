#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define CSV_PATH   "employees.csv"
#define CSV_HEADER "EmployeeName,Position,Salary,PaymentDate\n"

#define LINE_CAP 512
#define NAME_CAP 128
#define POS_CAP 128
#define DATE_CAP 32
#define NUM_CAP 64
#define CHOICE_CAP 64

#define NAME_COL 24
#define POS_COL 24
#define SAL_COL 14
#define DATE_COL 12

static void chomp(char *s){ s[strcspn(s, "\r\n")] = 0; }
static void ltrim(char *s){ size_t i=0; while (s[i] && isspace((unsigned char)s[i])) i++; if(i) memmove(s, s+i, strlen(s+i)+1); }
static void rtrim(char *s){ size_t n=strlen(s); while(n && isspace((unsigned char)s[n-1])) s[--n]=0; }
static void trim(char *s){ ltrim(s); rtrim(s); }

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
    char ans[NUM_CAP];
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
    char line[LINE_CAP];

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

        trim(tempName); trim(tempPosition); trim(tempSalary); trim(tempPaymentData);

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

    char new_name[NAME_CAP];
    char new_position[POS_CAP];
    char salary_input[NUM_CAP];
    float new_salary;
    char payment_date[DATE_CAP];
    
    // New Employee's Name
    printf("New name (Name Surname): ");
    fgets(new_name, sizeof new_name, stdin);
    chomp(new_name); trim(new_name);
    
    // New Employee's Position
    printf("Position: ");
    fgets(new_position, sizeof new_position, stdin);
    chomp(new_position); trim(new_position);

    // New Employee's Salary
    printf("Salary: ");
    fgets(salary_input, sizeof salary_input, stdin);
    chomp(salary_input); trim(salary_input);
    
    if (sscanf(salary_input, "%f", &new_salary) != 1) {
        printf("\n-------- WARNING ---------------------------\n");
        printf(" \033[33m Invalid input lil bro 🥀 \033[0m         \n");
        printf("--------------------------------------------\n\n");
        return;
    }

    // New Employee's Payment Data
    printf("Payment Date (YYYY-MM-DD): ");
    fgets(payment_date, sizeof payment_date, stdin);
    chomp(payment_date); trim(payment_date);

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

void search_record(void){
    char line[LINE_CAP], buffer[CHOICE_CAP], query[NAME_CAP];
    int found = 0, mode = 0;

    do {
        printf("Search by (1) Name (2) Position, (1-2): ");
        if (!fgets(buffer, sizeof buffer, stdin)) return;
    } while (sscanf(buffer, "%d", &mode) != 1 || (mode != 1 && mode != 2));

    printf("Keyword: ");
    if (!fgets(query, sizeof query, stdin)) return;
    chomp(query); trim(query);
    if (!*query){ puts("Empty keyword."); return; }

    FILE *file = fopen(CSV_PATH, "r");
    if (!file){ perror(CSV_PATH); return; }

    if (!fgets(line, sizeof line, file)){ puts("File Empty."); fclose(file); return; } // header

    printf("\n-----------------------------------------------------------------------------\n");
    printf("%-*s %-*s %*s %-*s\n", 
           NAME_COL, "EmployeeName", POS_COL, "Position",
           SAL_COL,  "Salary",       DATE_COL, "PaymentDate");
    printf("-----------------------------------------------------------------------------\n");

    while (fgets(line, sizeof line, file)){
        chomp(line);
        if (!*line) continue;

        char *name = strtok(line, ",");
        char *pos  = strtok(NULL, ",");
        char *sal  = strtok(NULL, ",");
        char *date = strtok(NULL, ",");

        if (!(name && pos && sal && date)) continue;
        trim(name); trim(pos); trim(sal); trim(date);

        int ok = (mode==1) ? (strcmp(name, query)==0) : (strcmp(pos, query)==0);
        if (ok){
            double salary_num = strtod(sal, NULL);
            printf("%-*s %-*s %*.*f %-*s\n",
                   NAME_COL, name, POS_COL, pos,
                   SAL_COL, 2, salary_num, DATE_COL, date);
            found++;
        }
    }

    if (!found) puts("No matches.");
    printf("-----------------------------------------------------------------------------\n\n");
    fclose(file);
}

void update_record(void){
    char target[NAME_CAP], line[LINE_CAP], buf[CHOICE_CAP];
    int field = 0, updated = 0;

    printf("Exact employee name to update: ");
    if (!fgets(target, sizeof target, stdin)) return;
    chomp(target); trim(target);
    if (!*target){ puts("Empty name. Cancelled."); return; }

    for (;;){
        printf("Update field: 1) Position  2) Salary  3) PaymentDate : ");
        if (!fgets(buf, sizeof buf, stdin)) return;
        if (sscanf(buf, "%d", &field)==1 && field>=1 && field<=3) break;
        puts("Enter 1, 2, or 3.");
    }

    FILE *in  = fopen(CSV_PATH, "r");
    if (!in){ perror(CSV_PATH); return; }
    FILE *out = fopen("temp.csv", "w");
    if (!out){ perror("temp.csv"); fclose(in); return; }

    if (fgets(line, sizeof line, in)) fputs(line, out);
    else { fclose(in); fclose(out); remove("temp.csv"); puts("File empty."); return; }

    while (fgets(line, sizeof line, in)){
        chomp(line);
        if (!*line) { fputc('\n', out); continue; }

        char *name = strtok(line, ",");
        char *pos  = strtok(NULL, ",");
        char *sal  = strtok(NULL, ",");
        char *date = strtok(NULL, ",");

        if (!(name && pos && sal && date)) continue;

        trim(name); trim(pos); trim(sal); trim(date);
        double salary_num = strtod(sal, NULL);

        if (strcmp(name, target) == 0){
            if (field == 1){
                char newpos[POS_CAP];
                printf("New position: ");
                if (fgets(newpos, sizeof newpos, stdin)){
                    chomp(newpos); trim(newpos);
                    if (*newpos) strcpy(pos, newpos);
                }
            } else if (field == 2){
                char sline[NUM_CAP];
                printf("New salary: ");
                if (fgets(sline, sizeof sline, stdin)){
                    double v;
                    if (sscanf(sline, "%lf", &v) == 1) salary_num = v;
                    else puts("Invalid number. Keeping old salary.");
                }
            } else {
                char newdate[DATE_CAP];
                printf("New date (YYYY-MM-DD): ");
                if (fgets(newdate, sizeof newdate, stdin)){
                    chomp(newdate); trim(newdate);
                    if (*newdate) strcpy(date, newdate);
                }
            }
            updated++;
        }

        fprintf(out, "%s,%s,%.2f,%s\n", name, pos, salary_num, date);
    }

    fclose(in); fclose(out);

    if (updated){
        remove(CSV_PATH);
        rename("temp.csv", CSV_PATH);
        printf("✓ Updated %d row(s).\n", updated);
    } else {
        remove("temp.csv");
        puts("Name not found. Nothing updated.");
    }
}

void delete_record(void){
    char target[NAME_CAP], line[LINE_CAP];
    int deleted = 0;

    printf("Exact name to delete: ");
    if (!fgets(target, sizeof target, stdin)) return;
    chomp(target); trim(target);
    if (!*target){ puts("Empty name."); return; }

    FILE *in  = fopen(CSV_PATH, "r");
    FILE *out = fopen("temp.csv", "w");
    if (!in || !out){ perror("open"); if(in)fclose(in); if(out)fclose(out); return; }

    if (fgets(line, sizeof line, in)) fputs(line, out);
    else { fclose(in); fclose(out); remove("temp.csv"); puts("Empty file."); return; }

    while (fgets(line, sizeof line, in)){
        chomp(line);
        if (!*line) continue;

        char *name = strtok(line, ",");
        char *pos  = strtok(NULL, ",");
        char *sal  = strtok(NULL, ",");
        char *date = strtok(NULL, ",");

        if (!(name && pos && sal && date)) continue;

        trim(name); trim(pos); trim(sal); trim(date);

        if (strcmp(name, target) == 0){
            char confirm[CHOICE_CAP]; int num = 2;
            for (;;){
                printf("Confirm Delete? : 1) Yes  2) No : ");
                if (!fgets(confirm, sizeof confirm, stdin)) break;
                if (sscanf(confirm, "%d", &num)==1 && (num==1 || num==2)) break;
                puts("Enter 1 or 2.");
            }
            if (num == 1){ deleted++; continue; }   // skip write = delete
            // else fall through and WRITE the row (cancel)
        }

        fprintf(out, "%s,%s,%s,%s\n", name, pos, sal, date);
    }

    fclose(in); fclose(out);

    if (deleted){
        remove(CSV_PATH);
        rename("temp.csv", CSV_PATH);
        printf("✓ Deleted %d row(s) named \"%s\".\n", deleted, target);
    } else {
        remove("temp.csv");
        puts("Name not found. Nothing deleted.");
    }
}


int main(void){
    char input_choice[CHOICE_CAP];
    int choice;

    check_csv();

    for (;;){
        display_data();
        printf("Enter your choice (1-7): ");
        if (!fgets(input_choice, sizeof input_choice, stdin)) { puts("\nbye!"); return 0; }
        chomp(input_choice); trim(input_choice);

        if (sscanf(input_choice, "%d", &choice) != 1 || choice < 1 || choice > 7){
            puts("\n\033[33mInvalid input lil bro 🥀\033[0m\n");
            continue;
        }

        switch (choice){
            case 1: list_all(); break;
            case 2: create_record(); break;
            case 3: search_record(); break;
            case 4: update_record(); break;
            case 5: delete_record(); break;
            case 6: /* unused */ break;
            case 7: return 0;
        }
    }
}