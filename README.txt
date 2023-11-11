trans flag on the Wii (and maybe GameCube) in 512 bytes
does not support 576i/PAL displays, yet
480i version is above 512 bytes, blow me
should be run from the Homebrew Channel, other launchers not tested

DOL downloads at https://github.com/InvoxiPlayGames/WiiTrans/releases

to build, run make.bat (on windows, with devkitPPC installed), or run
  - powerpc-eabi-gcc -nostartfiles -nostdlib -nodefaultlibs
      -fno-builtin -Os -s trans.c -o transflag.bin
  - powerpc-eabi-objcopy -S -O binary transflag.bin
then fix the text0 load size in dolheader.bin (offset 0x90, 32-bit big
endian integer) and prefix transflag.bin with the header. that's a DOL!

TODO:
=====
this could almost definitely fit into a DOL that is under 256 bytes
if the stripe printing routine is translated into assembly, and if it
is fit into the DOL header entirely (which means the code must be 164
bytes or less, to account for load sizes which can't be anything but
zero)
