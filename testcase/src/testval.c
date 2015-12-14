#include "trap.h"

char a [10];

int main ()
{
	a[0] = 1;
	a[1] = 2;
	a[2] = 3;
	a[3] = 4;
	a[4] = 5;
	nemu_assert(a[1] == 0x1);
	nemu_assert(a[2] == 0x2);
	nemu_assert(a[3] == 0x3);
	nemu_assert(a[4] == 0x4);
	HIT_GOOD_TRAP;
}

