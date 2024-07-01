//program to create a new file and write into it

#include <fcntl.h>
#include <unistd.h>

int main() {
    int file_descriptor;
    mode_t mode;
    //mode holds the permissions or mode for the file being created. It specifies who can read, write, or execute the file
    
    //mode argument is used to alter the access permissions of the file being created. It's particularly useful when you want to control the access rights of the file to ensure proper security and access control in your application.
    mode = S_IRUSR | S_IWUSR | S_IXUSR;
    // the mode argument in the open() function call is used to specify the permissions for the newly created file, if the file doesn't already exist.

    // S_IRUSR: This constant represents the read permission for the owner (user) of the file. If this permission is set, the owner can read the contents of the file.

    // S_IWUSR: This constant represents the write permission for the owner (user) of the file. If this permission is set, the owner can modify or write to the file.

    // S_IXUSR: This constant represents the execute permission for the owner (user) of the file. If this permission is set, the owner can execute the file if it is an executable program or script.
     
    // S_IRGRP: Read permission for the group.
    // S_IWGRP: Write permission for the group.
    // S_IXGRP: Execute permission for the group.
    // S_IROTH: Read permission for others.
    // S_IWOTH: Write permission for others.
    // S_IXOTH: Execute permission for others.


    // S_IRWXU: Read, write, and execute permissions for the(user).
    // S_IRWXG: Read, write, and execute permissions for the group.
    // S_IRWXO: Read, write, and execute permissions for others.


    // Creating file with appropriate flags and permissions
    //To combine flags, the bitwise OR operator is used

    char file_path[] = "home/Scott/Documents/SourceC/new.txt";

    file_descriptor = open(file_path, O_CREAT | O_WRONLY, mode);

    // these flags are actually binary numbers, hence we can use the bitwise AND/OR/NOT operations on them

    // O_CREAT expand to binary notation - 0100
    // O_WRONLY expands to binary notation - 0001


    // in this case the open function does this with the flags:
    // O_CREATE: This flag indicates that if the file doesn't exist, it should be created.  
    //OR
    // O_WRONLY: This flag indicates that the file should be opened for writing only, not for reading.
    
    // Writing data to the file
    write(file_descriptor, "test data\n", 11);
    
    // Closing the file descriptor
    close(file_descriptor);
    
    return 0;
}
