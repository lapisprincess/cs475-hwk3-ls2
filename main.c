#include <stdlib.h>
#include <stdio.h>
#include "stack.h"
#include "ls2.h"

int main(int argc, char* argv[]) {
    if (argc < 2 || argc > 3) {
        printf("Usage: ./ls2 <path> [exact-match-pattern]\n");
        return 1;
    }
    int check;

    if (argc == 3) check = ls2(argv[1], argv[2], 1);
    else check = ls2(argv[1], "", 0);

    if (check) {
        printf("Invalid path\n");
        return 1;
    }

    return 0;
}
