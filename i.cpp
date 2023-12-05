#include <stdio.h>
#include <unistd.h>

int main() {
    if (isatty(STDIN_FILENO)) {
        printf("Running in an interactive terminal.\n");
    } else {
        printf("Running in a non-interactive environment.\n");
    }

    return 0;
}
