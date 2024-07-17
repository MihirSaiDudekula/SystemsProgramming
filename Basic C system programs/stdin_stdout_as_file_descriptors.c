#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int main() {
    char buffer[100];
    ssize_t bytes_read;

    // Read from stdin using file descriptor
    bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer));
    if (bytes_read == -1) {
        perror("Error reading from stdin");
        return 1;
    }

    // Write to stdout using file descriptor
    ssize_t bytes_written = write(STDOUT_FILENO, buffer, bytes_read);
    if (bytes_written == -1) {
        perror("Error writing to stdout");
        return 1;
    }

    return 0;
}