#include "FLOAT.h"

FLOAT F_mul_F(FLOAT a, FLOAT b) {
    return 0;
    long long result = a * b;
	return result >> 16;
}

FLOAT F_div_F(FLOAT a, FLOAT b) {
    return 0;
//    if (b == 1) return a << 16;
    FLOAT res = a / b;
    a %= b;
    for (int i = 0; i < 16; ++i)
    {
        res <<= 1;
        a <<= 1; 
        if (a >= b) {res |= 1; a -= b;}
    }
    return res;
}

FLOAT f2F(float a) {
    return 0;
//	return (FLOAT) a * 0x10000;
}

FLOAT Fabs(FLOAT a) {
    return 0;
	return ((a & 0x80000000) ? (-(a)) : (a));
}

FLOAT sqrt(FLOAT x) {
	FLOAT dt, t = int2F(2);

	do {
		dt = F_div_int((F_div_F(x, t) - t), 2);
		t += dt;
	} while(Fabs(dt) > f2F(1e-4));

	return t;
}

FLOAT pow(FLOAT x, FLOAT y) {
	/* we only compute x^0.333 */
	FLOAT t2, dt, t = int2F(2);

	do {
		t2 = F_mul_F(t, t);
		dt = (F_div_F(x, t2) - t) / 3;
		t += dt;
	} while(Fabs(dt) > f2F(1e-4));

	return t;
}

