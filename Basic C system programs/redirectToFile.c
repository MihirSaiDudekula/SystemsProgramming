#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    // Open or create ping_results.txt for writing
    int file = open("ping_results.txt", O_WRONLY | O_CREAT, 0777);

    // Check for errors in opening file
    if (file == -1) {
        perror("Error opening file");
        return 1;
    }

    // Redirect stdout to the file descriptor 'file'
    dup2(file, STDOUT_FILENO);

    // Close the original file descriptor (optional)
    close(file);

    // Now stdout is redirected to ping_results.txt
    // Execute your program here

    return 0;
}
