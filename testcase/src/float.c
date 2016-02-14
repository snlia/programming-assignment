#include "trap.h"

void f (float a)
{
}

int main() {
    f (1.1);
	HIT_GOOD_TRAP;
	return 0;
}


