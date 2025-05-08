// main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	char **lines;
	size_t size;
} Reader;

Reader *createReader();
int read(Reader *c, const char *filename);
void print(Reader *c);
void cleanup(Reader *c);

int main(void) {
	Reader *r = createReader();
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

Reader *createReader() {
	Reader *r = malloc(sizeof(Reader));
	r->lines = NULL;
	r->size = 0;
	return r;
}

int read(Reader *r, const char *filename) {
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

void print(Reader *c) {
	for (int i = 0; i < c->size; i++) {
		printf("%d: %s\n", i + 1, c->lines[i]);
	}
}

void cleanup(Reader *c) {
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
