#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include "code-library.h"

Library *setup_lib(void) {
    DIR *d;
    if (!(d = opendir(DIR_PATH))) {
        printf("\033[%dmOpening Directory %s failed.\033[0m\n", RED, DIR_PATH);
        exit(EXIT_FAILURE);
    }
    int buffer = 10;
    Library *lib = malloc(sizeof(Library) + sizeof(struct entry[buffer]));
    if (lib == NULL) exit(0);
    int count = 0;
    struct dirent *file;
    while ((file = readdir(d))) {
        if (file->d_type != 8) continue;
        if (count == buffer) {
            buffer *= 2;
            Library *tmp = realloc(lib, sizeof(Library) + sizeof(struct entry) * buffer);
            if (tmp == NULL) exit(EXIT_FAILURE);
            lib = tmp;
        }
        struct entry *e = &(lib->entries[count]);
        e->index = count + 1;
        char filename[PATH_LEN] = DIR_PATH;
        strcat(filename, file->d_name);
        strcpy(e->path, filename);
        FILE *f = fopen(filename, "r");
        f_gets(e->title, PATH_LEN, f);
        char s[PATH_LEN];
        f_gets(s, PATH_LEN, f); // empty line
        f_gets(e->tags, PATH_LEN, f);
        count++;
        fclose(f);
    }
    closedir(d);
    lib->len = count;
    Library *tmp = realloc(lib, sizeof(Library) + sizeof(struct entry) * lib->len);
    if (tmp == NULL) exit(EXIT_FAILURE);
    lib = tmp;
    sort_lib(lib);
    return lib;
}

void sort_lib(Library *lib) {
    qsort(lib->entries, lib->len, sizeof(struct entry), comp);
    for (int i = 0; i < lib->len; i++) lib->entries[i].index = i + 1;
}

int comp(const void *p1, const void *p2) {
    const struct entry *ps1 = p1;
    const struct entry *ps2 = p2;
    return strcmp(ps1->title, ps2->title); // sort by title
}

Library *search(const Library *lib, char *line) {
    line += 2;
    const struct entry *e = &(lib->entries[0]);
    const struct entry *end = e + lib->len;
    int count = 0;
    Library *search_lib = malloc(sizeof(Library) + sizeof(struct entry[lib->len]));
    while (e < end) {
        if (strstr(e->tags, line)) {
            search_lib->entries[count] = *e;
            count++;
        }
        e++;
    }
    search_lib->len = count;
    Library *tmp = realloc(search_lib, sizeof(Library) + sizeof(struct entry) * search_lib->len);
    if (tmp == NULL) exit(EXIT_FAILURE);
    search_lib = tmp;
    return search_lib;
}
