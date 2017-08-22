
int errno;

int *__errno_location(void);
int *__errno_location() { return &errno; }

