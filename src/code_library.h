#pragma once

#define PATH_LEN 80
#define TITLE_LEN 40
#define LINE_LEN 120
#define DIR_PATH "/home/joe/prog/c/code_library/library/"
#define DELIMITER_TOC "================================="
#define DELIMITER_ENTRY "-------------------------------"
#define LINE_SPACE 44
#define TOC 0
#define EXIT 667

#define RED 91
#define GREEN 92
#define CYAN 96
#define BLACK 40 // background
static const unsigned char colors[2] = {GREEN, CYAN};

/* a file representation according to template
        index: sequential num
        path: DIR_PATH + filename
        title: 1st line in file
        tags: 3rd line (categories to search by) */
struct entry {
    unsigned char index;
    char path[PATH_LEN];
    char title[TITLE_LEN];
    char tags[TITLE_LEN];
};

/* collection of entries */
typedef struct {
    unsigned char len;
    struct entry entries[];
} Library;

// main.c

void user_input(char *line);

void print_toc(const Library *lib);

unsigned char print_entry(const struct entry *entry);

int check_duplicate(const char *line);

void free_lib(void);

// library.c

Library *create_lib(void);

void sort_lib(Library *lib);

int comp(const void *p1, const void *p2);

Library *search_lib(const Library *lib, char *line);

// utils.c

void flags(char **argv);

void fgets_no_newline(char *s, int n, FILE *f);

void lower(const char *s, char *t);

void lower_and_trim(const char *s, char *t);
