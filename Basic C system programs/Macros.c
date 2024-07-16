#include <stdio.h>  // Include the standard input/output library

#define PI 3.14159  // Define a macro for the value of PI
#define SQUARE(x) ((x) * (x))  // Define a macro for calculating the square of a number

// Conditional compilation example
#define DEBUG 1

int main() {
    double radius = 5.0;
    double area;

    area = PI * SQUARE(radius);  // Use the defined macros

    printf("Area of the circle with radius %.2f is %.2f\n", radius, area);

#ifdef DEBUG
    printf("Debugging is enabled. Radius: %.2f, Area: %.2f\n", radius, area);
#endif

    return 0;
}
