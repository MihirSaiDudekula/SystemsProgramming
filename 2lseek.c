// Using the lseek() system call
// Lseek system call is used to navigate to different positions within an open file

//lseek() is usually used for files with records of fixed sizes, common in older programming languages and large datasets.

// the file consists of records, which are lines, each of the same size 

// hence if each record is 16 bits, to access the first line it is the first 16 bits, then 16-32 bits is second record ..so on



// lets say we use the file data.txt

// Record 1: Lorem ipsum           
// Record 2: dolor sit
// Record 3: amet, consectetur
// Record 4: adipiscing elit
// Record 5: sed do eiusmod

// each of these records are at max 16 bits

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define RECORD_SIZE 16  
// Assuming each record is 16 bytes

int main() {
    // Open the file in read-only mode
    int fd = open("data.txt", O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // Move to the third record
    off_t offset = lseek(fd, (RECORD_SIZE + 1) * 2, SEEK_SET); 
    // we move ((16+1)*2) bytes, the +1 for \n after each record ,which is 2 records ahead, to directly access the third record from the start as specified by SEEK_SET

    // lseek takes 3 arguments FD, offset and whence
    // offset is how much offset do we want
    // whence is used to define from where to implement this offset

    // whence is of 3 types
    // SEEK_SET - offset from the start
    // SEEK_CUR - offset from the currnt location
    // SEEK_END - offset from the end

    //lseek returns the offset as an integer back, which can be stored simply in a int variable or more appropriately in a off_t variable

    //offset = -1 indicates a failure in seeking

    // remember - lseek only moves the seek or the cursor around the opened file
    //It cannot read/write anything in the file
    
    if (offset == -1) {
        perror("Error seeking file");
        close(fd);
        exit(EXIT_FAILURE);
    }

    printf("\n%d",offset);

    // Read the third record
    char buffer[RECORD_SIZE];
    ssize_t num_read = read(fd, buffer, RECORD_SIZE);

    if (num_read == -1) {
        perror("Error reading file");
        close(fd);
        exit(EXIT_FAILURE);
    }

    printf("\n%d",num_read);

    // Null-terminate the buffer
    buffer[num_read] = '\0';

    // Print the third record
    printf("\nThird Record: %s\n", buffer);

    // Close the file
    if (close(fd) == -1) {
        perror("Error closing file");
        exit(EXIT_FAILURE);
    }

    return 0;
}
