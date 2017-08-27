exc = ["DE", "DB", "NMI", "BP", "OF", "BR", "UD", "NM", "DF", "EXC9", "TS", "NP", "SS", "GP", "PF", "EXC15", "MF", "AC", "MC", "XF"]
err = [8, 10, 11, 12, 13, 14, 17]
print '''#include \"dt.h\"
#include \"basic_defs.h\"

'''

for i in range(256):
        if i < len(exc):
                haserr = i in err
                name = exc[i]
        else:
                haserr = False
                name = "IRQ%s" % i
        print "extern const char idt_entry_%s[];" % name
        if haserr:
                print("asm (\".code64\\n.globl idt_entry_%s\\nidt_entry_%s:\\npush $%s\\njmp idt_common_asm\\n.code32\\n\");" % (name, name, i))
        else:
                print("asm (\".code64\\n.globl idt_entry_%s\\nidt_entry_%s:\\npush $0\\npush $%s\\njmp idt_common_asm\\n.code32\\n\");" % (name, name, i))
print '''
#define OFFS(off) (((((uint64_t)(off)) << 32) & 0xffff000000000000ull) | ((off) & 0xffffull))
#define SEG(cs) ((cs) << 16)

static inline Gate IDTDESC(const void *handler)
{
    Gate ret;

    ret.hi = HIDWORD(PTR_TO_PA(handler));
    ret.lo = ((HIWORD(PTR_TO_PA(handler)) << 48) | LOWORD(PTR_TO_PA(handler))) |
        DT_P | DT_DPL(0) | DT_TYPE(INTGATE64) | SEG(DT_KERN64_CODE_SEL) | DT_IST(0);
    return ret;
}
'''

print "void init_idt_entries(Gate idt[static 256])\n{"
for i in range(256):
        if i < len(exc):
                haserr = i in err
                name = exc[i]
        else:
                haserr = False
                name = "IRQ%s" % i
        print "    idt[%s] = IDTDESC(idt_entry_%s);" % (i, name)
print "}\n"
