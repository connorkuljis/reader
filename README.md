# reader

Dynamically allocated file reader in C using `getline(3)`.

```c
struct Reader *createReader();
int read(struct Reader *c, const char *filename);
void print(struct Reader *c);
```

* Inspired by [https://pkg.go.dev/io#Reader](https://pkg.go.dev/io#Reader)

