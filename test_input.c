#include <stdio.h>
#include <stdlib.h>

struct Point {
    int x;
    int y;
};

int main() {
    int unusedVar;
    int normal = 5;
    int uninit;
    int *ptr = malloc(sizeof(int));

    // String icindeki // yorum olarak silinmemeli
    char *text = "// bu yorum degil";

    printf("Normal: %d\n", normal);
    printf("Uninit: %d\n", uninit);

    free(ptr);
    printf("Ptr: %d\n", *ptr);

    while (1) {
        break;
    }

    return 0;
}
