CPUID(VENDOR0,          0, 0, EBX,  0, 32)
CPUID(VENDOR1,          0, 0, EDX,  0, 32)
CPUID(VENDOR2,          0, 0, ECX,  0, 32)
CPUID(SSE3,             1, 0, ECX,  0,  1)
CPUID(APICID,           1, 0, EBX, 24,  8)
CPUID(MWAIT,            1, 0, ECX,  3,  1)
CPUID(VMX,              1, 0, ECX,  5,  1)
CPUID(X2APIC,           1, 0, ECX, 21,  1)
CPUID(HASMSR,           1, 0, EDX,  5,  1)
CPUID(MWAITHASEXT,      5, 0, ECX,  0,  1)
CPUID(MWAITEXTINTR,     5, 0, ECX,  1,  1)

CPUID(LAHF64,  0x80000001, 0, ECX,  0,  1)
CPUID(SYSCALL, 0x80000001, 0, EDX, 11,  1)
CPUID(NX,      0x80000001, 0, EDX, 20,  1)
CPUID(GBPAGE,  0x80000001, 0, EDX, 26,  1)
CPUID(RDTSCP,  0x80000001, 0, EDX, 27,  1)
CPUID(LM,      0x80000001, 0, EDX, 29,  1)
CPUID(INVTSC,  0x80000007, 0, EDX,  8,  1)
CPUID(NPT,     0x8000000A, 0, EDX,  0,  1)

CPUID(X2APICID,       0xB, 0, EDX, 0,  32)
CPUID(FSGSBASE,         7, 0, EBX, 0,   1)
CPUID(MSRFSGSBASE,0x80000001,0, EDX, 29,  1)
