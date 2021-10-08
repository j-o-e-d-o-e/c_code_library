#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

#define LLEN 70
#define DIR_PATH "/media/joe/E/programming/c/code_library/library/"
#define DELIMITER_TOC "====================================="
#define DELIMITER_ENTRY "-------------------------------------"

void fget(char *string, int n, FILE *file);

void toc(void);

void entry(int i);

DIR *d;
struct dirent *file;

int main(void) {
    toc();
    while (1) {
        printf("\nWhat would you like to read? ");
        int ch;
        scanf("%d", &ch);
        if (ch == 667) {
            printf("Devil's neighbour wishes you a good day.\n");
            break;
        } else if (ch == 0) {
            printf("\n");
            toc();
            continue;
        }
        entry(ch);
    }
    return 0;
}

void toc(void) {
    if (!(d = opendir(DIR_PATH))) {
        printf("Open Dir failed.\n");
        exit(EXIT_FAILURE);
    }
    printf("%s%s%s\n", DELIMITER_TOC, "C CODE LIBRARY", DELIMITER_TOC);
    int count = 1;
    while ((file = readdir(d))) {
        if (file->d_type != 8) continue;

        char tmp[] = DIR_PATH;
        char *p = strcat(tmp, file->d_name);
        FILE *f = fopen(p, "r");
        char h[LLEN];
        fget(h, LLEN, f);
        char s[LLEN];
        fget(s, LLEN, f); // empty line
        fget(s, LLEN, f);
        int n = count < 10 ? 19 : 20;
        printf("%d - %-*s-> (%d) %s\n", count, LLEN - n, h, count, s);
        count++;
        fclose(f);
    }
    closedir(d);
}

void entry(int i) {
    printf("\n%s\n", DELIMITER_ENTRY);
    i += 2;
    d = opendir(DIR_PATH);
    int c = 1;
    while ((file = readdir(d))) {
        if (file->d_type != 8 || c != i) {
            c++;
            continue;
        }
        FILE *f;
        char base[] = DIR_PATH;
        char *p = strcat(base, file->d_name);
        if ((f = fopen(p, "r")) == NULL) {
            printf("Opening File %s failed.\n", file->d_name);
            exit(EXIT_FAILURE);
        }
        char h1[LLEN];
        fget(h1, LLEN, f);
        printf("%s\n\n", h1);
        char s1[LLEN];
        fget(s1, LLEN, f); // empty line
        fget(s1, LLEN, f); // src
        fget(s1, LLEN, f); // empty line
        while (1) {
            base[0] = '\0';
            fgets(base, LLEN, f);
            printf("%s", base);
            if (feof(f)) break;
        }
        fclose(f);
        printf("%s\n", DELIMITER_ENTRY);
        break;
    }
    closedir(d);
}

void fget(char *string, int n, FILE *f) {
    // wrapper for fgets - replaces
    // first newline with null character

    fgets(string, n, f);

    while (*string != '\0') {
        if (*string == '\n') {
            *string = '\0';
            break;
        }
        string++;
    }
}
