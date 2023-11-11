static inline void write16(unsigned int addr, unsigned short x)
{
	asm("sth %0,0(%1);" : : "r"(x), "b"(0xc0000000 | addr));
}
static inline void write8(unsigned int addr, unsigned char x)
{
	asm("stb %0,0(%1);" : : "r"(x), "b"(0xc0000000 | addr));
}
static inline void write32(unsigned int addr, unsigned int x)
{
	asm("stw %0,0(%1);" : : "r"(x), "b"(0xc0000000 | addr));
}
static inline void eieio() {
	asm("eieio;");
}
static inline void nop() {
	asm("nop;");
}
#include <math.h>

// default is 480p, uncomment to use 480i PAL60/NTSC mode
//#define CRINGE_AND_UNBASED 1
// 576i PAL50 has fallen.

// physical MEM1 address of framebuffer
#define FB_ADDR 0x160000
// width and height of framebuffer
#define FB_WIDTH 640
#define FB_HEIGHT 480
// 5 stripes for the trans flag, easy enough to change
#define NUM_STRIPES 5
#define STRIPE_HEIGHT FB_HEIGHT / NUM_STRIPES

void _start()
{
	// reactivate VI
	// do as little as possible
#ifdef CRINGE_AND_UNBASED
	// 480i interlaced
	// address of second field
	// set up framebuffer
	write32(0x0C00201C, (FB_ADDR >> 5) | 0x10000000);
	write32(0x0C002024, (FB_ADDR + (2 * FB_WIDTH) >> 5) | 0x10000000);
	write16(0x0C002000, 0x0F06);
	write16(0x0C00200C, 0x0003);
	write16(0x0C002010, 0x0002);
	write16(0x0C00200E, 0x0018);
	write16(0x0C002012, 0x0019);
	// not worth trying to fix this because 480i sucks
	// deal with the bigger sizes or john Wii U
#else
	// 480p progressive scan
	// doing this all in ASM saves bytes!
	// GCC y u no optimise??
	// original C:
	/*write32(0x0C002024, (FB_ADDR >> 5) | 0x10000000);
	write16(0x0C002000, 0x1E0c);
	write16(0x0C00200C, 0x0006);
	write16(0x0C002010, 0x0006);
	write16(0x0C00200E, 0x0030);
	write16(0x0C002012, 0x0030);*/
	asm(
		"lis 10, 0xCC00;" \
		// set up vertical timing
		"li 9, 0x1E0C;" \
		"sth 9, 0x2000(10);" \
		// set vblank interval
		"li 9, 0x0006;" \
		"li 8, 0x0030;" \
		"sth 9, 0x200C(10);" \
		"sth 9, 0x2010(10);" \
		"sth 8, 0x200E(10);" \
		"sth 8, 0x2012(10);" \
		// set framebuffer address
		// calculate through (FB_ADDR >> 5) | 0x10000000
		"lis 9, 0x1000;" \
		"ori 9, 9, 0xB000;" \
		"stw 9, 0x201C(10);" \
		"stw 9, 0x2024(10);" \
	);
#endif
	eieio();
	// draw the stripes to the framebuffer
	// i'm too lazy to write this in assembly i'm sorry
	for (int stripe = 0; stripe < NUM_STRIPES; stripe++) {
		for (int line = (stripe * STRIPE_HEIGHT); line < STRIPE_HEIGHT + (stripe * STRIPE_HEIGHT); line++) {
			for (int i = 0; i < FB_WIDTH * 2; i += 4) {
				// all colours in yuyv422
				if (stripe == 0 || stripe == 4) // blue
					write32(FB_ADDR + (line * FB_WIDTH * 2) + i, 0xA8A4A84A);
				else if (stripe == 1 || stripe == 3) // pink
					write32(FB_ADDR + (line * FB_WIDTH * 2) + i, 0xB67BB6A0);
				else if (stripe == 2) // white
					write32(FB_ADDR + (line * FB_WIDTH * 2) + i, 0xEB80EB80);
			}
		}
	}
	// jumps to STUBHAXX stub at 0x80001800 if it's loaded
	// else we just loop
	asm(
		"lis 10, 0x8000;" \
		"lwz 9, 0x1800(10);" \
		"cmpwi 9, 0;" \
		"beq 0x0;" \
		"ori 10, 10, 0x1800;" \
		"mtctr 10;" \
		"bctr;" 
	);
}
