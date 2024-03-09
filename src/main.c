#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "code-library.h"

int main(int argc, char **argv) {
    if (argc > 1) flags(argv);
    Library *lib = create_lib();
    user_input(lib);
    free(lib);
    return EXIT_SUCCESS;
}

void user_input(Library *lib) {
    print_toc(lib);
    using_history();
    char prompt[TITLE_LEN];
    sprintf(prompt, "\n\033[%dmWhat would you like to read? \033[0m", RED);
    for (;;) {
        char *line = readline(prompt);
        if (strstr(line, "s:") == line) {
            Library *tmp = search_lib(lib, line + 2);
            if (tmp->len > 0) {
                if (!check_duplicate(line)) add_history(line);
                printf("\n");
                print_toc(tmp);
            } else {
                printf("\033[%dmNo match found.\033[0m", RED);
                printf("\n");
            }
            free(tmp);
            free(line);
            continue;
        }
        char *error;
        long ch = strtol(line, &error, 10);
        if (*error == *line) {
            printf("\033[%dmNot a number.\033[0m", RED);
            printf("\n");
            free(line);
        } else if (ch == TOC) {
            printf("\n");
            print_toc(lib);
            free(line);
        } else if (ch == EXIT) {
            printf("\033[%dmDevil's neighbour wishes you a good day.\033[0m\n", RED);
            free(line);
            break;
        } else if ((ch > lib->len) != 0) {
            printf("\033[%dmNot a valid number.\033[0m", RED);
            printf("\n");
            free(line);
        } else {
            struct entry *entry = &lib->entries[ch - 1];
            char item[TITLE_LEN + sizeof(short) + 2];
            sprintf(item, "%hu (%s)", (unsigned short) ch, entry->title);
            if (strlen(line) > 0 && !check_duplicate(item)) add_history(item);
            print_entry(entry);
            free(line);
        }
    }
}

void print_toc(const Library *lib) {
    printf("\033[%d;1m%s %s %s\033[0m\n", RED, DELIMITER_TOC, "C CODE LIBRARY", DELIMITER_TOC);
    const struct entry *current = &(lib->entries[0]);
    const struct entry *end = current + lib->len;
    while (current < end) {
        if (current->index % 2 != 0)
            printf("\033[%dm%*d - %-*s%s\033[0m\n", GREEN, 2, current->index, LINE_SPACE,
                   current->title, current->tags);
        else
            printf("\033[%d;%dm%*d - %-*s%s\033[0m\n", CYAN, BLACK, 2, current->index, LINE_SPACE,
                   current->title, current->tags);
        current++;
    }
}

void print_entry(const struct entry *entry) {
    FILE *f;
    if ((f = fopen(entry->path, "r")) == NULL) {
        printf("\033[%dmOpening File %s failed.\033[0m\n", RED, entry->title);
        exit(EXIT_FAILURE);
    }
    printf("\n\033[%dm%s\033[0m\n", RED, DELIMITER_ENTRY);
    char title[TITLE_LEN];
    fgets(title, TITLE_LEN, f);
    printf("\033[%d;1;4m%d - %s\033[0m\n", colors[entry->index % 2 == 0], entry->index, title);
    for (unsigned char i = 0; i < 3; i++) fgets((char[TITLE_LEN]) {0}, TITLE_LEN, f);
    char line[LINE_LEN];
    while (!feof(f)) {
        line[0] = '\0';
        fgets(line, LINE_LEN, f);
        if (strcmp(line, "EXAMPLE\n") == 0) printf("\033[%d;1m%s\033[0m", colors[entry->index % 2 == 0], line);
        else printf("\033[%dm%s\033[0m", colors[entry->index % 2 == 0], line);
    }
    fclose(f);
    printf("\033[%dm%s\033[0m\n", RED, DELIMITER_ENTRY);
}

int check_duplicate(char *line) {
    HIST_ENTRY **entry = history_list();
    for (int i = 0; i < history_length; i++) if (strcmp(entry[i]->line, line) == 0) return 1;
    return 0;
}
