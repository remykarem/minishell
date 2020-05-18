// Program to copy a file

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFFER_LEN 4096

int main(int argc, char *argv[])
{
    int src_fd, dst_fd, nbytes, err;
    char buffer[BUFFER_LEN];

    // Assume that the program takes two arguments:
    // the source path followed by the destination path.
    if (argc != 3)
    {
        printf("Usage: copy-one source_path destination_path\n\n");
        printf("Copy a file to a destination.\n\n");
        printf("Warning: This overwrites any existing file ");
        printf("at specified destination.\n");
        exit(EXIT_FAILURE);
    }

    // WARNING: this overwrites any existing file
    // at file destination!
    src_fd = open(argv[1], O_RDONLY);
    dst_fd = open(argv[2], O_CREAT | O_WRONLY, 0644);

    while (1)
    {
        // Read file BUFFER_LEN bytes at a time
        nbytes = read(src_fd, buffer, BUFFER_LEN);
        if (nbytes == -1)
        {
            printf("Error reading file %s.\n", argv[1]);
            exit(EXIT_FAILURE);
        }

        // Finish reading; pointer has nothing else to read
        if (nbytes == 0)
            break;

        nbytes = write(dst_fd, buffer, nbytes);
        if (nbytes == -1)
        {
            printf("Error writing to file %s.\n", argv[2]);
            exit(EXIT_FAILURE);
        }
    }

    close(src_fd);
    close(dst_fd);
}
