#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <readline/history.h>
#include <readline/readline.h>

#define NBYTES 128
#define MAX_BUFFER 1024
#define FILENAME_BUFFER 256
#define NUM_PREVIEW_FILES 10
#define RED printf("\033[0;31m");
#define CYAN printf("\033[0;36m");
#define GREEN printf("\033[0;32m");
#define YELLOW printf("\033[0;33m");
#define DEFAULT_COLOUR printf("\033[0m");
#define DASHES printf("------------\n");

void open_directory(char *path_to_dir, int num_preview_files)
{
	DIR *dir;
	int nbytes;
	int view_fd;
	int counter = 0;
	char buffer[MAX_BUFFER];
	struct stat stat_buffer;
	struct dirent *dir_entry;
	int reading_buffer = NBYTES;
	char *filename = (char *)malloc(FILENAME_BUFFER * sizeof(char));
	char *full_file_path = (char *)malloc(1 + strlen(path_to_dir) + strlen(filename));

	dir = opendir(path_to_dir);
	if (dir == NULL)
	{
		perror("Error");
		exit(EXIT_FAILURE);
	}

	// Assume first two are always `.`  and `..`
	readdir(dir);
	readdir(dir);

	// Prints `num_preview_files` files in the directory
	CYAN DASHES
	YELLOW
	while ((dir_entry = readdir(dir)) != NULL &&
		   counter++ < num_preview_files)
		printf("─ %s\n", dir_entry->d_name);
	CYAN DASHES
	printf("\n");

	// Enter interpreter
	printf("Enter filename to view its contents.\n");
	printf("Hit Enter to view the next 10 files.\n");
	printf("Hit Ctrl-C to quit.\n");
	while (1)
	{
		// Get filename from the given list
		filename = readline("\033[1;36m>>> ");
		if (filename == NULL)
		{
			perror("Error reading input");
			exit(1);
		}

		if (strcmp(filename, ".") == 0)
		{
			RED
			printf("That is here!\n");
			DEFAULT_COLOUR
			continue;
		}
		else if (strcmp(filename, "") == 0)
		{
			// Prints `num_preview_files` files in the directory
			counter = 0;
			CYAN DASHES
			YELLOW
			while ((dir_entry = readdir(dir)) != NULL &&
				   counter++ < num_preview_files)
				printf("- %s\n", dir_entry->d_name);
			CYAN DASHES
			printf("\n");
			continue;
		}

		// Get full path to filename
		strcpy(full_file_path, path_to_dir);
		strcat(full_file_path, filename);
		printf("Opening: %s\n", full_file_path);

		// Open file
		view_fd = open(full_file_path, O_RDONLY);
		if (view_fd < 0)
		{
			RED
			printf("Error opening file.\n");
			DEFAULT_COLOUR
			continue;
		}

		fstat(view_fd, &stat_buffer);
		if (S_ISDIR(stat_buffer.st_mode) == 1)
		{
			RED
			printf("That's a directory.\n");
			DEFAULT_COLOUR
			close(view_fd);
			continue;
		}

		// Get file size
		if (stat_buffer.st_size < NBYTES)
			reading_buffer = stat_buffer.st_size;

		// Read and print file contents
		nbytes = read(view_fd, buffer, reading_buffer);
		if (nbytes == -1)
		{
			RED
			printf("Error reading file %s.\n", full_file_path);
			DEFAULT_COLOUR
			close(view_fd);
			continue;
		}
		GREEN
		printf("%.*s\n", reading_buffer, buffer);
		DEFAULT_COLOUR

		close(view_fd);
	}

	free(filename);
	free(full_file_path);
	closedir(dir);
}

int main(int argc, char *argv[])
{
	int num_preview_files = NUM_PREVIEW_FILES;

	if (argc == 3)
	{
		sscanf(argv[2], "%d", &num_preview_files);
	}
	else if (argc != 2)
	{
		printf("Usage: list path_to_dir [num_preview_files]\n\n");
		printf("List a specified number of files from a ");
		printf("specified directory.");
		exit(EXIT_FAILURE);
	}

	open_directory(argv[1], num_preview_files);

	return 0;
}
