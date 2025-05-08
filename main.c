// main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Reader {
	char **lines;
	size_t size;
};

struct Reader *createReader();
int read(struct Reader *c, const char *filename);
void print(struct Reader *c);
void cleanup(struct Reader *c);

int main(void) {
	struct Reader *r = createReader();
	if (r == NULL) {
		perror("error reading file content");
		exit(EXIT_FAILURE);
	}

	if (read(r, "content/hello.txt") == -1) {
		exit(EXIT_FAILURE);
	}

	if (read(r, "content/hello.txt2") == -1) {
		exit(EXIT_FAILURE);
	}
	print(r);

	cleanup(r);

	return 0;
}

struct Reader *createReader() {
	struct Reader *r = malloc(sizeof(struct Reader));
	r->lines = NULL;
	r->size = 0;
	return r;
}

int read(struct Reader *r, const char *filename) {
	FILE *f = fopen(filename, "r");
	if (!f) {
		perror("error opening file");
		return -1;
	}

	char *lineBuf = NULL;
	size_t lineBufSize = 0;
	int n = 0;

	while (getline(&lineBuf, &lineBufSize, f) != -1) {
		// Reallocate memory for the array of lines.
		r->lines = realloc(r->lines, (r->size + 1) * sizeof(char *));
		if (!r->lines) {
			perror("allocation error");
			cleanup(r);
			return -1;
		}

		// Allocate memory for the line, and copy line buffer content.
		r->lines[r->size] = malloc((strlen(lineBuf) + 1) * sizeof(char));
		if (!r->lines[r->size]) {
			perror("allocation error");
			cleanup(r);
			return -1;
		}
		strcpy(r->lines[r->size], lineBuf);

		// Replace new line characters with null terminator if non-empty.
		size_t read = strlen(r->lines[r->size]);
		if (read > 0 && r->lines[r->size][read - 1] == '\n') {
			r->lines[r->size][read - 1] = '\0';
		}

		n += read;
		r->size++;
	}
	free(lineBuf);

	fclose(f);

	return n;
}

void print(struct Reader *c) {
	for (int i = 0; i < c->size; i++) {
		printf("%d: %s\n", i + 1, c->lines[i]);
	}
}

void cleanup(struct Reader *c) {
	if (!c || !c->size) {
		return;
	}

	if (c->lines) {
		for (int i = 0; i < c->size; i++) {
			free(c->lines[i]);
		}
		free(c->lines);
	}
	free(c);
}
