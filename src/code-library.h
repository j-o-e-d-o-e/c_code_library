#pragma once

#define S_LEN 120
#define DIR_PATH "/home/joe/temp/c-test/code_library/library/"
#define DELIMITER_TOC "================================="
#define DELIMITER_ENTRY "-------------------------------"
#define TOC 0
#define EXIT 667
#define clear() printf("\033[H\033[J")

#define RED 91
#define GREEN 92
#define CYAN 96
#define BLACK 40 // background
static const int colors[2] = {GREEN, CYAN};

/* a file representation according to template
        index: sequential num
        path: DIR_PATH + filename
        title: 1st line in file
        tags: 3rd line (category) */
struct entry {
    int index;
    char path[S_LEN];
    char title[S_LEN];
    char tags[S_LEN]; // literature
};

/* collection of entries */
typedef struct {
    int len;
    struct entry entries[];
} Library;

// main.c

void user_input(Library *lib);

void print_toc(const Library *lib);

void print_entry(const struct entry *entry);

int check_duplicate(char* line);

// library.c

Library *setup_lib(void);

void sort_lib(Library *lib);

int comp(const void *p1, const void *p2);

Library *search(const Library *lib, char *line);

// utils.c

void flags(char **argv);

/* wrapper for fgets(): replaces first newline with null character */
void f_gets(char *s, int n, FILE *f);

void lower(const char *s, char *t);

void lower_and_trim(const char *s, char *t);
