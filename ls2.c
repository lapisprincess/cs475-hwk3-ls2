#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>     // dirent struct
#include <sys/stat.h>   // file info
#include <unistd.h>     // read dirs
#include "ls2.h"

int ls2(char *path, char *pattern, int pat_srch) {
    struct stack_t *outstack = initstack();
    _ls2(outstack, path, pattern, pat_srch, 0);
    printstack(outstack);
    freestack(outstack);
    return 0;
}

void _indent(char *line, int lvl) {
    char *buf = (char*) malloc(strlen(line) + (lvl * strlen(INDENT)) + 20);;
    for (int i = 0; i < lvl; i++) {
        strcpy(buf, INDENT);
        strcat(buf, line);
        strcpy(line, buf);
    }
    free(buf);
}

int _ls2(struct stack_t *outstack, char *path, char *pattern, int pat_srch, int lvl) {
    DIR *dirp = opendir(path);
    struct dirent *dp;
    struct stat buf;
    char *full_path;
    char *str;
    while (dirp) {
        dp = readdir(dirp);
        if (dp == NULL) break;
        if (!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, "..")) continue;
        full_path = (char*) malloc(strlen(path) + strlen(dp->d_name) + 20);
        sprintf(full_path, "%s/%s", path, dp->d_name);
        lstat(full_path, &buf);
        if (S_ISDIR(buf.st_mode)) { // folder
            str = (char*) malloc((strlen(INDENT) * lvl) + strlen(dp->d_name) + 20);
            sprintf(str, "%s/ (directory)", dp->d_name);
            _indent(str, lvl);
            if (pat_srch == 0) {
                push(outstack, str);
                _ls2(outstack, full_path, pattern, pat_srch, lvl + 1);
            } else if (_ls2(outstack, full_path, pattern, pat_srch, lvl + 1) == 1) {
                // if it wasn't 11:00pm on a Friday, I might try to get the full path...
                // but alas, you'll have to deal with my half-assery. sorry!
                push(outstack, str);
            }
        } else if (S_ISREG(buf.st_mode)) { // regular file
            str = (char*) malloc((strlen(INDENT) * lvl) + strlen(dp->d_name) + 20);
            sprintf(str, "%s (%jd bytes)", dp->d_name, buf.st_size);
            _indent(str, lvl);
            if (pat_srch == 1 && strcmp(dp->d_name, pattern) == 0) {
                push(outstack, str);
                return 1;
            } else if (pat_srch == 0) push(outstack, str);
        }
    }
    closedir(dirp);
    return 0;
}
