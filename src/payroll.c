#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifndef CSV_PATH
#define CSV_PATH "employees.csv"
#endif

#ifndef CSV_HEADER
#define CSV_HEADER "EmployeeName,Position,Salary,PaymentDate\n"
#endif

#define LINE_CAP   512
#define NAME_CAP   128
#define POS_CAP    128
#define DATE_CAP   32 
#define NUM_CAP    64
#define CHOICE_CAP 64

#define NAME_COL 24
#define POS_COL  24
#define SAL_COL  14
#define DATE_COL 12

/* ----------------- string utils ----------------- */
static void chomp(char *s){ s[strcspn(s, "\r\n")] = 0; }
static void ltrim(char *s){ size_t i=0; while (s[i] && isspace((unsigned char)s[i])) i++; if(i) memmove(s, s+i, strlen(s+i)+1); }
static void rtrim(char *s){ size_t n=strlen(s); while(n && isspace((unsigned char)s[n-1])) s[--n]=0; }
static void trim(char *s){ ltrim(s); rtrim(s); }

/* ----------------- i/o helpers ------------------ */
static void clear_line(void){
    int c = getchar();
    if (c == EOF) return;
    if (c != '\n') {
        ungetc(c, stdin);
        while ((c = getchar()) != '\n' && c != EOF) {}
    }
}

/* ========== tiny test helpers (AAA style) ========== */
static void ut_write_file(const char *path, const char *text){
    FILE *f = fopen(path, "w");
    if (!f) { perror(path); exit(1); }
    fputs(text, f);
    fclose(f);
}
static char* ut_slurp(const char *path){
    FILE *f = fopen(path, "rb");
    if (!f) { perror(path); exit(1); }
    fseek(f,0,SEEK_END); long n = ftell(f); rewind(f);
    char *buf = (char*)malloc((size_t)n+1);
    fread(buf,1,(size_t)n,f); buf[n]=0; fclose(f);
    return buf;
}
static FILE* ut_redirect_stdin(const char *path){
    FILE *in = freopen(path, "r", stdin);
    if (!in) { perror("freopen(stdin)"); exit(1); }
    return in;
}
static void ut_restore_stdin(void){
    /* mac/linux */
    freopen("/dev/tty","r",stdin);
}
static int ut_total=0, ut_pass=0;
static void ut_assert_contains(const char *hay, const char *needle, const char *msg){
    ++ut_total;
    if (strstr(hay, needle)){ printf("  ✅ %s\n", msg); ++ut_pass; }
    else                    { printf("  ❌ %s\n     expected: [%s]\n", msg, needle); }
}
static void ut_assert_true(int cond, const char *msg){
    ++ut_total;
    if (cond){ printf("  ✅ %s\n", msg); ++ut_pass; }
    else     { printf("  ❌ %s\n", msg); }
}

/* extra tiny utils used by e2e */
static int file_exists(const char *p){ FILE *f=fopen(p,"r"); if(!f) return 0; fclose(f); return 1; }
static int count_csv_lines(const char *p){ FILE*f=fopen(p,"r"); if(!f) return 0; int c, n=0; while((c=fgetc(f))!=EOF) if(c=='\n') ++n; fclose(f); return n; }

/* -------------------------------------------------- */

static void check_csv(void){
    char ans[NUM_CAP];
    int choice;

    FILE *file = fopen(CSV_PATH, "r");
    if (!file) {
        perror("Couldn't open CSV (will offer to create)");
        printf("Do you wish to create new employees file?\n");
        printf("1) Yes  2) No\n");
        printf("Input: ");

        if (!fgets(ans, sizeof ans, stdin)) return;
        chomp(ans);

        /* only 1..2 are valid here */
        if (sscanf(ans, "%d", &choice) != 1 || choice < 1 || choice > 2) {
            puts("\n\033[33mInvalid input lil bro 🥀\033[0m\n");
            return;
        }
        if (choice == 1) {
            FILE *newFile = fopen(CSV_PATH, "w");
            if (!newFile) { perror("create CSV"); return; }
            fputs(CSV_HEADER, newFile);
            fclose(newFile);
        } else {
            puts("K bro, find the CSV then.");
        }
    } else {
        fclose(file);
    }
}

static void list_all(void){
    char line[LINE_CAP];
    FILE *file = fopen(CSV_PATH, "r");
    if (!file) { perror(CSV_PATH); return; }

    if (!fgets(line, sizeof line, file)) { puts("File Empty."); fclose(file); return; }

    printf("\n-----------------------------------------------------------------------------\n");
    printf("%-24s %-24s %-14s %-12s\n", "EmployeeName", "Position", "Salary", "PaymentDate");
    printf("-----------------------------------------------------------------------------\n");

    while (fgets(line, sizeof line, file)) {
        chomp(line);
        if (!*line) continue;

        char *name = strtok(line, ",");
        char *pos  = strtok(NULL, ",");
        char *sal  = strtok(NULL, ",");
        char *date = strtok(NULL, ",");

        if (!(name && pos && sal && date)) continue;
        trim(name); trim(pos); trim(sal); trim(date);

        printf("%-24s %-24s %-14s %-12s\n", name, pos, sal, date);
    }

    printf("-----------------------------------------------------------------------------\n\n");
    fclose(file);
}

static void display_data(void){
    printf("===============================================================\n");
    printf("   \e[1m Employee Payroll System 🏦 \e[0m\n");
    printf("===============================================================\n");
    printf("1) List all record\n");
    printf("2) Create salary record\n");
    printf("3) Search salary record\n");
    printf("4) Update salary record\n");
    printf("5) Delete salary record\n");
    printf("6) Unit test [2] + E2E\n");
    printf("7) Exit\n");
    printf("===============================================================\n");
}

static void create_record(void){
    FILE *file = fopen(CSV_PATH, "a");
    if (!file) { perror(CSV_PATH); return; }

    char new_name[NAME_CAP];
    char new_position[POS_CAP];
    char salary_input[NUM_CAP];
    float new_salary;
    char payment_date[DATE_CAP];

    printf("New name (Name Surname): ");
    if (!fgets(new_name, sizeof new_name, stdin)) { fclose(file); return; }
    chomp(new_name); trim(new_name);

    printf("Position: ");
    if (!fgets(new_position, sizeof new_position, stdin)) { fclose(file); return; }
    chomp(new_position); trim(new_position);

    printf("Salary: ");
    if (!fgets(salary_input, sizeof salary_input, stdin)) { fclose(file); return; }
    chomp(salary_input); trim(salary_input);
    if (sscanf(salary_input, "%f", &new_salary) != 1) {
        puts("\n\033[33mInvalid salary 🥀\033[0m\n");
        fclose(file);
        return;
    }

    printf("Payment Date (YYYY-MM-DD): ");
    if (!fgets(payment_date, sizeof payment_date, stdin)) { fclose(file); return; }
    chomp(payment_date); trim(payment_date);

    fprintf(file, "%s,%s,%.2f,%s\n", new_name, new_position, new_salary, payment_date);

    if (fclose(file) == 0) {
        printf("\n--------------------------------------------\n");
        printf("\033[32m New Data Successfully Created.\033[0m\n");
        printf("--------------------------------------------\n\n");
    } else {
        puts("Cannot close the file");
    }
}

static void search_record(void){
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

    if (!fgets(line, sizeof line, file)){ puts("File Empty."); fclose(file); return; }

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

static void update_record(void){
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

static void delete_record(void){
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

/* ---------- Unit tests (menu 6) ---------- */
static void run_unit_tests(void){
    printf("\n==== UNIT TESTS (AAA) ====\n");

    /* backup */
    int had_backup = 0;
    if (file_exists(CSV_PATH)){
        remove("employees.bak");
        if (rename(CSV_PATH,"employees.bak")==0) had_backup = 1;
    }

    /* UT1: create_record appends normalized row */
    puts("\n[UT1] create_record() should append a normalized row");
    ut_write_file(CSV_PATH, CSV_HEADER);
    ut_write_file("ut_in1.txt",
        "Employee 1\n"
        "Developer\n"
        "42000.5\n"
        "2025-08-01\n"
    );
    ut_redirect_stdin("ut_in1.txt");
    create_record();
    ut_restore_stdin();

    char *csv1 = ut_slurp(CSV_PATH);
    ut_assert_contains(csv1, "Employee 1,Developer,42000.50,2025-08-01\n",
                       "Row appended with salary formatted to 2 decimals");
    free(csv1);
    remove("ut_in1.txt");

    /* UT2: update_record changes Position for exact name */
    puts("\n[UT2] update_record() should change Position for exact name");
    ut_write_file(CSV_PATH,
        CSV_HEADER
        "Employee 1,Developer,42000.50,2025-08-01\n"
        "Employee 2,Manager,55500.00,2025-08-05\n"
    );
    ut_write_file("ut_in2.txt",
        "Employee 1\n"
        "1\n"
        "Senior Developer\n"
    );
    ut_redirect_stdin("ut_in2.txt");
    update_record();
    ut_restore_stdin();

    char *csv2 = ut_slurp(CSV_PATH);
    ut_assert_contains(csv2, "Employee 1,Senior Developer,42000.50,2025-08-01\n",
                       "Position updated for Employee 1");
    ut_assert_contains(csv2, "Employee 2,Manager,55500.00,2025-08-05\n",
                       "Other rows unaffected");
    free(csv2);
    remove("ut_in2.txt");

    printf("\n==== UNIT SUMMARY: %d/%d passed ====\n", ut_pass, ut_total);

    /* restore */
    remove(CSV_PATH);
    if (had_backup) rename("employees.bak",CSV_PATH);
    else remove("employees.bak");
}

/* ---------- E2E (menu 6) ---------- */
static void run_e2e_test(void){
    const char *testfile = "test_employees.csv";
    const char *names[]     = { "Employee 1", "Employee 2", "Employee 3" };
    const char *positions[] = { "Senior Developer", "HR Manager", "Financial Analyst" };
    const char *salaries[]  = { "60000.00", "55000.00", "45000.00" };
    const char *dates[]     = { "2025-02-01", "2025-02-02", "2025-02-03" };
    const int N = 3;

    printf("\n=== E2E: add -> save -> read -> assert ===\n");

    /* Arrange: write CSV */
    FILE *w = fopen(testfile,"w"); fputs(CSV_HEADER, w);
    for (int i=0;i<N;i++) fprintf(w,"%s,%s,%s,%s\n",names[i],positions[i],salaries[i],dates[i]);
    fclose(w);

    /* Act: read back */
    FILE *r = fopen(testfile,"r");
    char line[LINE_CAP]; fgets(line,sizeof line,r); /* header */
    char rn[3][NAME_CAP], rp[3][POS_CAP], rs[3][NUM_CAP], rd[3][DATE_CAP];
    int got=0;
    while (got<N && fgets(line,sizeof line,r)){
        chomp(line);
        char *n=strtok(line,","), *p=strtok(NULL,","), *s=strtok(NULL,","), *d=strtok(NULL,",");
        if (!(n&&p&&s&&d)) continue;
        trim(n); trim(p); trim(s); trim(d);
        strcpy(rn[got],n); strcpy(rp[got],p); strcpy(rs[got],s); strcpy(rd[got],d);
        got++;
    }
    fclose(r);

    /* Assert */
    ut_assert_true(file_exists(testfile),"CSV file was created");
    ut_assert_true(got==N,"Employee count after read");
    for(int i=0;i<N;i++){
        ut_assert_true(strcmp(names[i],rn[i])==0,"name matches");
        ut_assert_true(strcmp(positions[i],rp[i])==0,"position matches");
        ut_assert_true(strcmp(salaries[i],rs[i])==0,"salary matches");
        ut_assert_true(strcmp(dates[i],rd[i])==0,"date matches");
    }
    ut_assert_true(count_csv_lines(testfile)==N+1,"CSV line count (incl header)");

    remove(testfile);
}

/* ================================================== */

#ifndef TESTING
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
            case 1: list_all();       break;
            case 2: create_record();  break;
            case 3: search_record();  break;
            case 4: update_record();  break;
            case 5: delete_record();  break;
            case 6: run_unit_tests(); run_e2e_test(); break;
            case 7: return 0;
        }
    }
}
#endif
