#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int num_files = 0;
	char src_folder[256];
	char dst_folder[256];

	DIR *dir;
	struct dirent *dir_ent;
	int counter = 2;
	int src_fd, dst_fd, n, err;
	char buffer[4096];
	char *src_path, dst_path;

	sscanf(argv[1], "%d", &num_files);
	num_files += 2; // Includes "."  and ".." directories

	dir = opendir(argv[2]);
	if (dir)
	{
		while ((dir_ent = readdir(dir)) != NULL && counter < num_files)
		{
			snprintf(src_folder, sizeof src_folder, "%s%s", argv[2], dir_ent->d_name);
			snprintf(dst_folder, sizeof dst_folder, "%s%s", argv[3], dir_ent->d_name);

			printf("%s\n", argv[2]);
			printf("%s\n", src_folder);
			printf("%s\n", dst_folder);

			src_fd = open(src_folder, O_RDONLY);
			dst_fd = open(dst_folder, O_CREAT | O_WRONLY);

			// if (src_fd < 0) {
			//   perror("Cannot open src folder");
			//   exit(1);
			// }
			if (dst_fd < 0)
			{
				perror("Cannot open dst folder");
				exit(1);
			}

			while (1)
			{
				err = read(src_fd, buffer, 4096);
				if (err == -1)
				{
					perror("Error reading file");
					exit(1);
				}
				n = err;

				if (n == 0)
					break;

				err = write(dst_fd, buffer, n);
				if (err == -1)
				{
					perror("Error writing to file");
					exit(1);
				}
			}

			close(src_fd);
			close(dst_fd);

			counter++;
		}
		closedir(dir);
	}

	return 0;
}
