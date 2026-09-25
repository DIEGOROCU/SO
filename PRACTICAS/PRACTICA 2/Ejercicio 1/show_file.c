#include <stdio.h>
#include <stdlib.h>
#include <err.h>

int main(int argc, char* argv[]) {
	FILE* file = NULL;
	int c, ret;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s <file_name>\n", argv[0]);
		exit(1);
	}

	if ((file = fopen(argv[1], "r")) == NULL)
		err(2, "The input file %s could not be opened", argv[1]);

	while ((c = getc(file)) != EOF) {
		ret = putc((unsigned char) c, stdout);

		if (ret == EOF) {
			fclose(file);
			err(3, "putc() failed!!");
		}
	}

	fclose(file);
	return 0;
}
