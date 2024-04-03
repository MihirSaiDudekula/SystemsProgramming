//A simple C program that reads and prints the contents of a text file 


#include <stdio.h>
//contains input output functions like - printf, scanf, etc.

#include <fcntl.h>
//is a library function used for file control
//usually works with file descriptors
//File Descriptor (FD) is usually an Integer value used to uniquely identify an opened file.

#include <unistd.h>
//a standard library used to interact with the OS

#define BUFFER_SIZE 1024
//our buffer is where we want to temporarily store the obtained data
//we choose a suitable buffer size which >= required size for comfortable accomodation

int main(int argc, char *argv[]) {
    //argc stores the number of commandline arguments passed during execution
    //argv[NUM] can be used to access these argument values ,like arg[2]

    int fd;
    // our file descriptor

    char buffer[BUFFER_SIZE];
    // our buffer, which is a string of the defined size

    fd = open(argv[1], O_RDONLY);
    // open function takes in 2 arguments - file path and flag
    // file path refers to the name/path of the file
    // the second argument is the flag which specifies how the file should be opened/ dealt with. some flags can be- O_RDONLY, O_WRONLY, or O_RDWR.  These request opening the file read-only, write-only, or read/write, respectively.
    // the open function returns the file descriptor if the opened file
    // this FD can be used subseuently to read/write etc. from the file


    if (fd != -1) 
    {
        ssize_t num_read;
        //signed size type - is a datatype which stores size of object, typically the byte size of the return of read() and write(), which return the size of the file read. It should be signed becuase the returned value can be negative which indicates errors


        while ((num_read = read(fd, buffer, BUFFER_SIZE - 1)) > 0) {
        // num_read stores the number of bytes read as returned by the read function 
        // here we specify BUFFER SIZE-1 which is 1023 bytes.
        //so here , we first read 1023 bytes , then the 1024th byte is set to \0 to indicate end of string 

            buffer[num_read] = '\0';
            // we print the 1023+1 bytes read from the buffer, then we repeat this loop, until all the bytes in the text file are read, and each time read 1024 bytes, print, read the next 1024 bytes..so on

            printf("%s\n", buffer);
        }
        close(fd);
        // use the close function which takes the FD to close the file
    } 
    else 
    {
        perror("Error opening file");
        //perror is a printf like function which is used to print errors during system call failures
        return -1;
    }
    return 0;
}
