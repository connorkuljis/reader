# reader

Dynamically allocated file reader in C using `getline(3)`.

```c
Reader *createReader();
int read(Reader *c, const char *filename);
void print(Reader *c);
```

* Inspired by [https://pkg.go.dev/io#Reader](https://pkg.go.dev/io#Reader)

