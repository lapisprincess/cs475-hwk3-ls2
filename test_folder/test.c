#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>     // dirent struct
#include <sys/stat.h>   // file info
#include <unistd.h>     // read dirs
#include "ls2.h"

int ls2(char *path, stack_t *out) 
{ return _ls2(path, "", out, 0); }

int ls2_pat(char *path, char *pattern, stack_t *out)
{ return _ls2(path, pattern, out, 0); }

void _indent(char *line, int lvl) {
    for (int i = 0; i < lvl; i++) {
        char buf[285];
        strcpy(buf, INDENT);
        strcat(buf, line);
        strcpy(line, buf);
    }
}

int _ls2(char *path, char *pattern, stack_t *out, int lvl) {
    printf("current path: %s\n", path);
    DIR *dirp = opendir(path);
    char str[285];
    struct dirent *dp;
    struct stat buf;

    while (dirp) {
        dp = readdir(dirp);
        if (dp == NULL) return 1;
        if (dp->d_name[0] == '.') continue;

        lstat(dp->d_name, &buf);
        if (S_ISDIR(buf.st_mode)) { // folder
            sprintf(str, "%s/ (directory)", dp->d_name);
            printf("%s\n", str);
            char full_path[285];
            sprintf(full_path, "%s/%s", path, dp->d_name);
            _ls2(full_path, pattern, out, lvl + 1);
        } else if (S_ISREG(buf.st_mode)) { // regular file
            sprintf(str, "%s (%jd bytes)", dp->d_name, buf.st_size);
            _indent(str, lvl);
            printf("%s\n", str);
            push(out, &str);
        }
    }
    closedir(dirp);
    return 0;
}
