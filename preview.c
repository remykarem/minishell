#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <readline/readline.h>
#include <readline/history.h>

#define NUM_FILES 10
#define NBYTES 128
#define FILENAME_BUFFER 256
#define MAX_BUFFER 1024

void open_directory(char *filename_, int num_files)
{
	DIR *dir;
	int nbytes;
	int counter = 0;
	int view_fd;
	char buffer[MAX_BUFFER];
	struct stat stat_buffer;
	struct dirent *dir_entry;
	int reading_buffer = NBYTES;
	char *filename = (char *)malloc(FILENAME_BUFFER * sizeof(char));
	char *full_file_path = (char *)malloc(1 + strlen(filename_) + strlen(filename));

	dir = opendir(filename_);
	if (dir == NULL)
	{
		perror("Error");
		exit(EXIT_FAILURE);
	}

	// Assume first two are always `.`  and `..`
	readdir(dir);
	readdir(dir);

	// Prints `num_files` files in the directory
	printf("\033[0;36m");
	printf("------------\n");
	printf("\033[0;33m");
	while ((dir_entry = readdir(dir)) != NULL &&
		   counter++ < num_files)
		printf("─ %s\n", dir_entry->d_name);
	printf("\033[0m");
	printf("\033[0;36m");
	printf("------------\n\n");

	// Enter interpreter
	printf("Enter filename to view its contents.\n");
	printf("Hit Enter to view the next 10 files.\n");
	printf("Hit Ctrl-C to quit.\n");
	while (1)
	{
		// Get filename from the given list
		filename = readline("\033[1;36m>>> ");
		if (filename == NULL)
			exit(1);

		if (strcmp(filename, ".") == 0)
		{
			printf("\033[0;31mThat is here!\033[0m\n");
			continue;
		}
		else if (strcmp(filename, "") == 0)
		{
			// Prints `num_files` files in the directory
			counter = 0;
			printf("\033[0;36m");
			printf("------------\n");
			printf("\033[0;33m");
			while ((dir_entry = readdir(dir)) != NULL &&
				   counter++ < num_files)
				printf("- %s\n", dir_entry->d_name);
			printf("\033[0m");
			printf("\033[0;36m");
			printf("------------\n\n");
			continue;
		}

		// Get full path to filename
		strcpy(full_file_path, filename_);
		strcat(full_file_path, filename);
		printf("Opening: %s\n", full_file_path);

		// Open file
		view_fd = open(full_file_path, O_RDONLY);
		if (view_fd < 0)
		{
			printf("\033[0;31mError opening file.\033[0m\n");
			continue;
		}

		fstat(view_fd, &stat_buffer);
		if (S_ISDIR(stat_buffer.st_mode) == 1)
		{
			printf("\033[0;31mThat's a directory.\033[0m\n");
			close(view_fd);
			continue;
		}

		// Get file size
		if (stat_buffer.st_size < NBYTES)
			reading_buffer = stat_buffer.st_size;

		// Read and print file
		nbytes = read(view_fd, buffer, reading_buffer);
		if (nbytes == -1)
		{
			printf("\033[0;31m");
			printf("Error reading file %s.\033[0m\n", filename_);
			close(view_fd);
			continue;
		}
		printf("\033[0;32m%.*s\n", reading_buffer, buffer);
		printf("\033[0m");

		close(view_fd);
	}

	free(filename);
	free(full_file_path);
	closedir(dir);
}

int main(int argc, char *argv[])
{
	int num_files = NUM_FILES;

	if (argc == 3)
	{
		sscanf(argv[2], "%d", &num_files);
	}
	else if (argc != 2)
	{
		printf("Usage: list path_to_dir [num_files]\n\n");
		printf("List a specified number of files from a ");
		printf("specified directory.");
		exit(EXIT_FAILURE);
	}

	open_directory(argv[1], num_files);

	return 0;
}
