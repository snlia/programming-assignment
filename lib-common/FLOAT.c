#include "FLOAT.h"

FLOAT F_mul_F(FLOAT a, FLOAT b) {
    long long result = a * b;
	return result >> 16;
}

FLOAT F_div_F(FLOAT a, FLOAT b) {
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
    unsigned int x = *((unsigned int *) &a); // change point type to unsigned int to get the compute code of a
    if (x == 0xc7000000) return 0x80000000; // return if x = -0x8000
    unsigned int Significand = x & 0x7fffff;
    x >>= 23;
    int Exponent = x & 0xff - 0x7f;
    x >>= 8;
    unsigned int Sign = x & 1;
    //pick up Sign, Exponent and Significand...
    //now we have float a = (Sign) 1.Significand * 2^Exponent, we need to get a = result * 2^-16
    FLOAT result = 0; 
    if (Exponent <= -17) return 0; //FLOAT can only store abs (value) >= 1e-16, 
    Significand |= 0x800000;
    Significand <<= 7; //now a = (Sign) Significand * 2^(Exponent - 30); result = (Sign) Significand * 2^ (Exponent - 14)
    if (Exponent > 14) ; //float point overflow
    result = Significand >> (14 - Exponent);
    if (result & 1 && Exponent != 14 && (Significand >> (13 - Exponent)) & 1) result += 1;
    return Sign ? -result : result;
}

FLOAT Fabs(FLOAT a) {
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

