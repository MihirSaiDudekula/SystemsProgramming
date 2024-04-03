#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>

int main() {
    DIR *dp;
    //dp is a pointer to a diretory which is a structure DIR
    //this pointer points to open folder, which contains the entire collection of subfiles and subfolders
    //Functions like opendir() and closedir() work with DIR pointers

    struct dirent *entry;
    // dirent is a structure that stores information regarding the file
    // like its name, date of creation, size etc
    // the *entry pointer points to the individual subfile which can be looped over for obtaining details of all subfiles/subfolders


    dp = opendir(".");
    //open current directory and dp points to it


    if (dp == NULL) {
        printf("Error opening directory\n");
        return 1;
    }


    while ((entry = readdir(dp)) != NULL) 
    {
        // loop through the contents of dp one by one and for each, we want to list out the name of the files
        if(entry->d_type==DT_REG)
        {
            // a special check whic filters out only the regular type files 
            printf("%s\n", entry->d_name);
        }
    }

    if (closedir(dp) == -1) {
        printf("Error closing directory\n");
        return 1;
    }

    return 0;
}
