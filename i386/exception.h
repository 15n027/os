#ifndef EXCEPTION_H
#define EXCEPTION_H

enum {
    EXC_DE = 0,
    EXC_DB,
    EXC_NMI,
    EXC_BP,
    EXC_OF,
    EXC_BR = 5,
    EXC_UD,
    EXC_NM,
    EXC_DF = 8,
    EXC_TS = 10,
    EXC_NP,
    EXC_SS,
    EXC_GP,
    EXC_PF,
    EXC_MF = 16,
    EXC_AC,
    EXC_MC,
    EXC_XF,
};

#endif
