#!/usr/bin/env python
exc = ["DE", "DB", "NMI", "BP", "OF", "BR", "UD", "NM", "DF", "IRQ9", "TS", "NP", "SS", "GP", "PF", "IRQ15", "MF", "AC", "MC", "XM", "VE"]
err = [8, 10, 11, 12, 13, 14, 17]

for i in range(256):
        if i < len(exc):
                haserr = i in err
                name = exc[i]
        else:
                haserr = False
                name = "IRQ%s" % i
        print "ENTRY(%6s, %3s, %10s, %6s)" % (
                name, i, "INTGATE", "HASERR" if haserr else "NOERR")
