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
		exit(EXIT_FAILURE);
	}

	if (read(r, "content/t8.shakespeare.txt") == -1) {
		exit(EXIT_FAILURE);
	}

	print(r);

	cleanup(r);

	return 0;
}

struct Reader *createReader() {
	struct Reader *r = malloc(sizeof(struct Reader));

	if (!r) {
		perror("error allocating reader");
		return NULL;
	}

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
		// Bumps lines capacity by one.
		r->lines = realloc(r->lines, (r->size + 1) * sizeof(char *));

		if (!r->lines) {
			perror("error: unable to allocate lines.");
			cleanup(r);
			return -1;
		}

		// Allocate enough memory for current line, including null terminator.
		r->lines[r->size] = malloc((strlen(lineBuf) + 1) * sizeof(char));

		if (!r->lines[r->size]) {
			perror("error: unable to allocate line.");
			cleanup(r);
			return -1;
		}

		// Copy line buffer contents to current line (indexed by current size).
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
	if (!c) {
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
