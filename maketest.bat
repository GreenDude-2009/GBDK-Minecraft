@echo off
c:\gbdk\bin\lcc -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -c -o test.o test.c
c:\gbdk\bin\lcc -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -o test.gb test.o
del test.lst
del test.map
del test.o
del test.sym
del jump.gb
ren test.gb jump.gb
@echo on