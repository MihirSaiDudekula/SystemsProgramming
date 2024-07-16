#include <stdio.h>
#include <string.h>

int main() {
    int arr1[] = {3, 4};
    int arr2[] = {1, 2};
    
    // Example of memcmp
    if (memcmp(arr1, arr2, 2 * sizeof(int)) == 0) {
        printf("Arrays are identical\n");
    } else {
        printf("Arrays are not identical\n");
    }
    
    // Example of memcpy
    memcpy(arr1, arr2, 2 * sizeof(int));
    printf("arr1 after memcpy: %d, %d\n", arr1[0], arr1[1]);
    
    // Example of memset
    memset(arr1, 0, 2 * sizeof(int));
    printf("arr1 after memset: %d, %d\n", arr1[0], arr1[1]);
    
    // Example of memchr
    int *ptr = memchr(arr2, 1, 2 * sizeof(int));
    if (ptr != NULL) {
        printf("Found the byte\n");
    } else {
        printf("Did not find the byte\n");
    }
    
    return 0;
}
