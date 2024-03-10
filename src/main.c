#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "code_library.h"

Library *library;

int main(int argc, char **argv) {
    if (argc > 1) flags(argv);
    library = create_lib();
    atexit(free_lib);
    print_toc(library);
    using_history();
    char prompt[TITLE_LEN];
    sprintf(prompt, "\n\033[%dmWhat would you like to read? \033[0m", RED);
    for (;;) {
        char *line = readline(prompt);
        user_input(line);
        free(line);
    }
}

void user_input(char *line) {
    if (strstr(line, "s:") == line) {
        Library *lib_search = search_lib(library, line + 2);
        if (lib_search->len > 0) {
            if (!check_duplicate(line)) add_history(line);
            puts("");
            print_toc(lib_search);
        } else {
            printf("\033[%dmNo match found.\033[0m", RED);
            puts("");
        }
        free(lib_search);
    } else {
        char *error;
        long num = strtol(line, &error, 10);
        if (*error == *line) {
            printf("\033[%dmNot a number.\033[0m", RED);
            puts("");
        } else if (num == TOC) {
            puts("");
            print_toc(library);
        } else if (num == EXIT) {
            printf("\033[%dmDevil's neighbour wishes you a good day.\033[0m\n", RED);
            free(line);
            exit(EXIT_SUCCESS);
        } else if (num < 0 || num > library->len) {
            printf("\033[%dmNot a valid number.\033[0m", RED);
            puts("");
        } else {
            const struct entry *entry = &library->entries[num - 1];
            char item[TITLE_LEN + sizeof(short) + 2];
            sprintf(item, "%hu (%s)", (unsigned short) num, entry->title);
            if (strlen(line) > 0 && !check_duplicate(item)) add_history(item);
            unsigned char ret = print_entry(entry);
            if (ret == 1){
                free(line);
                exit(EXIT_FAILURE);
            }
        }
    }
}

void print_toc(const Library *lib) {
    printf("\033[%d;1m%s %s %s\033[0m\n", RED, DELIMITER_TOC, "C CODE LIBRARY", DELIMITER_TOC);
    for (unsigned char i = 0; i < lib->len; i++) {
        const struct entry entry = lib->entries[i];
        if (i % 2 == 0)
            printf("\033[%dm%*d - %-*s%s\033[0m\n", GREEN, 2, entry.index, LINE_SPACE,
                   entry.title, entry.tags);
        else
            printf("\033[%d;%dm%*d - %-*s%s\033[0m\n", CYAN, BLACK, 2, entry.index, LINE_SPACE,
                   entry.title, entry.tags);
    }
}

unsigned char print_entry(const struct entry *entry) {
    FILE *f;
    if ((f = fopen(entry->path, "r")) == NULL) {
        printf("\033[%dmOpening File %s failed.\033[0m\n", RED, entry->path);
        return 1;
    }
    printf("\n\033[%dm%s\033[0m\n", RED, DELIMITER_ENTRY);
    printf("\033[%d;1;4m%d - %s\033[0m\n\n", colors[entry->index % 2 == 0], entry->index, entry->title);
    for (unsigned char i = 0; i < 4; i++) fgets((char[TITLE_LEN]) {0}, TITLE_LEN, f);
    char line[LINE_LEN];
    while (!feof(f)) {
        line[0] = '\0';
        fgets(line, LINE_LEN, f);
        if (strcmp(line, "EXAMPLE\n") == 0) printf("\033[%d;1m%s\033[0m", colors[entry->index % 2 == 0], line);
        else printf("\033[%dm%s\033[0m", colors[entry->index % 2 == 0], line);
    }
    fclose(f);
    printf("\033[%dm%s\033[0m\n", RED, DELIMITER_ENTRY);
    return 0;
}

int check_duplicate(const char *line) {
    HIST_ENTRY **entry = history_list();
    for (int i = 0; i < history_length; i++) if (strcmp(entry[i]->line, line) == 0) return 1;
    return 0;
}

void free_lib(void) {
    free(library);
}
