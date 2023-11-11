@echo off
del transflag.bin
del transflag.dol
C:\devkitPro\devkitPPC\bin\powerpc-eabi-gcc.exe -nostartfiles -nostdlib -nodefaultlibs -fno-builtin -Os -s trans.c -o transflag.bin
C:\devkitPro\devkitPPC\bin\powerpc-eabi-objcopy.exe -S -O binary transflag.bin
