extern void *ctors_begin;
extern void *ctors_end;

int main(void)
{
    void **p = &ctors_begin;

    while (p != &ctors_end) {
        void (*fn)(void) = (void(*)(void))*p;
        fn();
        p++;
    }
    asm("cli\nhlt\n");
    for(;;);
    return 0;
}
