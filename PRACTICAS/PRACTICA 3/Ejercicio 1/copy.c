#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

int copy(int fdo, int fdd)
{
    char buffer[512];
    ssize_t bytes_read;
    ssize_t bytes_written;

    while ((bytes_read = read(fdo, buffer, sizeof(buffer))) > 0) {
        bytes_written = write(fdd, buffer, bytes_read);

        if (bytes_written == -1) {
            return -1;
        }

        if (bytes_written != bytes_read) {
            errno = EIO;
            return -1;
        }
    }

    return bytes_read == -1 ? -1 : 0;
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source_file> <destination_file>\n", argv[0]);
        return 1;
    }

	int fdo = open(argv[1], O_RDONLY);
	if (fdo == -1) {
		perror("The source file could not be opened");
		return 2;
	}

	int fdd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (fdd == -1) {
		perror("The destination file could not be opened");
		close(fdo);
		return 3;
	}

	if (copy(fdo, fdd) == -1) {
		perror("The file could not be copied");
		close(fdo);
		close(fdd);
		return 4;
	}

	close(fdo);
	close(fdd);

	return 0;
}
