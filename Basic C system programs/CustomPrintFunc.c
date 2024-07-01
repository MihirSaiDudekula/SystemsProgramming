#include <stdarg.h> // for variadic functions
#include <stdio.h>

void my_printf(const char* format, ...) {
    va_list args; // declare a variable to hold the variable arguments
    va_start(args, format); // initialize 'args' to point to the first variable argument

    while (*format != '\0') { // loop until the end of the format string
        if (*format == '%') { // check if the current character is '%'
            format++; // Move to the next character after '%'
            switch (*format) { // start a switch statement based on the character following '%'
                case 'd': { // if the character following '%' is 'd'
                    int value = va_arg(args, int); // extract the next integer argument from 'args'
                    printf("%d", value); // print the integer value
                    break; // exit the switch statement
                }
                case 's': { // if the character following '%' is 's'
                    char* str = va_arg(args, char*); // extract the next string argument from 'args'
                    printf("%s", str); // print the string
                    break; // exit the switch statement
                }
                // Add more cases for other format specifiers as needed
                default: // if the character following '%' doesn't match any case
                    putchar(*format); // print the character directly
            }
        } else { // if the current character is not '%'
            putchar(*format); // print the character directly
        }
        format++; // Move to the next character in the format string
    }

    va_end(args); // clean up the 'args' variable argument list
}

int main() {
    my_printf("Hello, %s! Your favorite number is %d.\n", "John", 42); // call the custom printf function
    return 0;
}
