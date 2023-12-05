#include <stdio.h>
#include <unistd.h>

int main() {
    if (isatty(STDOUT_FILENO)) {
        printf("STDOUT is a terminal.\n");
    } else {
        printf("STDOUT is not a terminal.\n");
    }

    return 0;
}
