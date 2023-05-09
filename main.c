#include <stdio.h>
int main(){

    int array[5] = {1,2,3,4,5};
    array[2] = 5;

    printf("    array: %zu\n", array);
    printf("    array: %zu\n", *(array + 0));
    printf("    array: %zu\n", *(array + 1));
    printf("    array: %zu\n", *(array + 2));
    printf("    array: %zu\n", *(array + 3));
    printf("    array: %zu\n", *(array + 4));
    printf("\n");
    
    printf("    array: %zu\n", *(*(&array) + 2));
    printf("    array: %zu\n", &array);
    
    return 0;
}
