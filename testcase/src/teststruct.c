#include "trap.h"

typedef struct data
{
	int x, y;
	char z [2];
} A;

A a [10];

int main ()
{
	a[1].x = 0x123;
	a[2].y = 0x456;
	a[3].z[0] = 0x7;
	a[4].z[1] = 0x8;
	nemu_assert(a[1].x == 0x123);
	nemu_assert(a[2].y == 0x456);
	nemu_assert(a[3].z[0] == 0x7);
	nemu_assert(a[4].z[1] == 0x8);
	HIT_GOOD_TRAP;
}
