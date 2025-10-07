#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


#define TESTING
#define CSV_PATH "e2e_employees.csv"
#include "payroll.c"   

static void write_file(const char *path, const char *text){
    FILE *f = fopen(path,"w"); assert(f); fputs(text,f); fclose(f);
}
static char* slurp(const char *path){
    FILE *f = fopen(path,"rb"); assert(f);
    fseek(f,0,SEEK_END); long n = ftell(f); rewind(f);
    char *buf = malloc((size_t)n+1); assert(buf);
    fread(buf,1,(size_t)n,f); buf[n]=0; fclose(f);
    return buf;
}
static void feed_stdin_from(const char *path){
    FILE *in = freopen(path,"r",stdin); assert(in);
}
static void restore_stdin(void){
    freopen("/dev/tty","r",stdin);
}
static int assert_contains(const char *hay, const char *needle, const char *msg){
    if (strstr(hay, needle)){ printf("  PASS - %s\n", msg); return 1; }
    printf("  FAIL - %s (missing: %s)\n", msg, needle); return 0;
}
static int count_lines(const char *path){
    FILE *f = fopen(path,"r"); if(!f) return 0;
    int c, n=0; while((c=fgetc(f))!=EOF) if(c=='\n') ++n; fclose(f); return n;
}

int main(void){
    int pass = 0, total = 0;

    //Arrange 
    remove(CSV_PATH);
    write_file(CSV_PATH, CSV_HEADER);
    printf("\n=== E2E TEST: add → update → delete ===\n");

    //Act: add 3 employees 
    write_file("in_add1.txt",
        "John Doe\nDeveloper\n42000.5\n2025-08-01\n");
    feed_stdin_from("in_add1.txt"); create_record(); restore_stdin();

    write_file("in_add2.txt",
        "Jane Smith\nHR Manager\n55000\n2025-08-05\n");
    feed_stdin_from("in_add2.txt"); create_record(); restore_stdin();

    write_file("in_add3.txt",
        "Alex Park\nFinancial Analyst\n60000\n2025-08-10\n");
    feed_stdin_from("in_add3.txt"); create_record(); restore_stdin();

    //Act: update Jane’s position -> Senior HR Manager 
    write_file("in_update.txt",
        "Jane Smith\n"      // exact name
        "1\n"               // field: 1) Position
        "Senior HR Manager\n");
    feed_stdin_from("in_update.txt"); update_record(); restore_stdin();

    //Act: delete John (confirm yes) 
    write_file("in_delete.txt",
        "John Doe\n"  // target
        "1\n");       // confirm yes
    feed_stdin_from("in_delete.txt"); delete_record(); restore_stdin();

    //Assert 
    char *csv = slurp(CSV_PATH);

    total++; pass += assert_contains(csv,
        CSV_HEADER, "CSV header present");

    total++; pass += assert_contains(csv,
        "Alex Park,Financial Analyst,60000.00,2025-08-10\n",
        "Row: Alex present (normalized salary)");

    total++; pass += assert_contains(csv,
        "Jane Smith,Senior HR Manager,55000.00,2025-08-05\n",
        "Row: Jane updated to Senior HR Manager");

    total++; pass += (strstr(csv, "John Doe,Developer,42000.50,2025-08-01\n")==NULL)
        ? (printf("  PASS - Row: John removed\n"), 1)
        : (printf("  FAIL - Row: John still present\n"), 0);

    //3 rows now: header + Jane + Alex = 3 lines 
    int lines = count_lines(CSV_PATH);
    total++; pass += (lines == 3)
        ? (printf("  PASS - Line count = 3\n"), 1)
        : (printf("  FAIL - Line count expected 3, got %d\n", lines), 0);

    free(csv);

    //Cleanup 
    remove("in_add1.txt"); remove("in_add2.txt"); remove("in_add3.txt");
    remove("in_update.txt"); remove("in_delete.txt");
    remove(CSV_PATH);

    printf("\nE2E SUMMARY: %d/%d passed\n", pass, total);
    return (pass==total) ? 0 : 1;
}
