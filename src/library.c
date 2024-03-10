#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "code_library.h"

Library *create_lib(void) {
    DIR *d;
    if (!(d = opendir(DIR_PATH))) {
        printf("\033[%dmOpening Directory %s failed.\033[0m\n", RED, DIR_PATH);
        exit(EXIT_FAILURE);
    }
    unsigned char buffer = 10;
    Library *lib = malloc(sizeof *lib + sizeof(struct entry[buffer]));
    if (lib == NULL) exit(EXIT_FAILURE);
    unsigned char count = 0;
    struct dirent *file;
    while ((file = readdir(d))) {
        if (file->d_type != 8) continue;
        if (count == buffer) {
            buffer += 10;
            Library *tmp = realloc(lib, sizeof(Library) + sizeof(struct entry) * buffer);
            if (tmp == NULL) {
                free(lib);
                exit(EXIT_FAILURE);
            } else lib = tmp;
        }
        struct entry *entry = &(lib->entries[count]);
        char filename[PATH_LEN] = DIR_PATH;
        strcat(filename, file->d_name);
        strcpy(entry->path, filename);
        FILE *f = fopen(filename, "r");
        fgets_no_newline(entry->title, TITLE_LEN, f);
        fgets((char[TITLE_LEN]) {0}, TITLE_LEN, f);
        fgets_no_newline(entry->tags, TITLE_LEN, f);
        fclose(f);
        count++;
    }
    closedir(d);
    lib->len = count;
    Library *tmp = realloc(lib, sizeof(Library) + sizeof(struct entry) * lib->len);
    if (tmp == NULL) {
        free(lib);
        exit(EXIT_FAILURE);
    }
    lib = tmp;
    sort_lib(lib);
    return lib;
}

void sort_lib(Library *lib) {
    qsort(lib->entries, lib->len, sizeof(struct entry), comp);
    for (unsigned char i = 0; i < lib->len; i++) lib->entries[i].index = i + 1;
}

int comp(const void *p1, const void *p2) {
    const struct entry *ps1 = p1;
    const struct entry *ps2 = p2;
    return strcmp(ps1->title, ps2->title);
}

Library *search_lib(const Library *lib, char *line) {
    char search_term[TITLE_LEN];
    trim(search_term, line);
    Library *lib_search = malloc(sizeof *lib_search + sizeof(struct entry[lib->len]));
    if (lib_search == NULL) {
        free(line);
        exit(EXIT_FAILURE);
    }
    const struct entry *current = &(lib->entries[0]);
    const struct entry *end = current + lib->len;
    unsigned char count = 0;
    while (current < end) {
        if (strstr(current->tags, search_term)) lib_search->entries[count++] = *current;
        current++;
    }
    lib_search->len = count;
    Library *tmp = realloc(lib_search, sizeof(Library) + sizeof(struct entry) * lib_search->len);
    if (tmp == NULL) {
        free(line);
        free(lib_search);
        exit(EXIT_FAILURE);
    }
    lib_search = tmp;
    return lib_search;
}
