#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define TESTING
#include "payroll.c"

// ---------- helpers ----------
static void write_file(const char *path, const char *text){
    FILE *f = fopen(path, "w"); assert(f); fputs(text, f); fclose(f);
}
static char* slurp(const char *path){
    FILE *f = fopen(path, "rb"); assert(f);
    fseek(f,0,SEEK_END); long n = ftell(f); rewind(f);
    char *buf = malloc((size_t)n+1); assert(buf);
    fread(buf,1,(size_t)n,f); buf[n]=0; fclose(f);
    return buf;
}
static void feed_stdin_from(const char *path){
    FILE *in = freopen(path, "r", stdin);
    assert(in != NULL);
}

static void test_create_record_appends_row(void){
    // Arrange
    write_file(CSV_PATH, CSV_HEADER);
    write_file("tmp.in",
        "John Doe\n"
        "Developer\n"
        "42000.5\n"
        "2025-08-01\n"
    );
    feed_stdin_from("tmp.in");

    // Act
    create_record();

    // Assert
    char *csv = slurp(CSV_PATH);
    assert(strstr(csv, "John Doe,Developer,42000.50,2025-08-01\n") != NULL);
    free(csv);
    remove("tmp.in");
}

static void test_update_record_changes_position(void){
    // Arrange
    write_file(CSV_PATH,
        CSV_HEADER
        "John Doe,Developer,42000.50,2025-08-01\n"
        "Jane Smith,Manager,55500.00,2025-08-05\n"
    );
    write_file("tmp.in",
        "John Doe\n"   // target name
        "1\n"          // field = Position
        "Senior Developer\n"
    );
    feed_stdin_from("tmp.in");

    // Act
    update_record();

    // Assert
    char *csv = slurp(CSV_PATH);
    assert(strstr(csv, "John Doe,Senior Developer,42000.50,2025-08-01\n") != NULL);
    assert(strstr(csv, "Jane Smith,Manager,55500.00,2025-08-05\n") != NULL); // untouched
    free(csv);
    remove("tmp.in");
}

int main(void){
    // clean
    remove(CSV_PATH);

    test_create_record_appends_row();
    test_update_record_changes_position();

    puts("All unit tests passed ✅");
    // cleanup
    remove(CSV_PATH);
    return 0;
}
