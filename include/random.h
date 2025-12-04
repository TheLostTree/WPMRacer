#ifndef RANDOM_H_
#define RANDOM_H_
#include <stdint.h>


// xorshift32 alg has small teeny memory footprint and decent randomness for non-crypto use
typedef struct{
    uint32_t state;
} xorshift32;

uint32_t next_rand32(xorshift32 *state)
{
	uint32_t x = state->state;
	x ^= x << 13; x ^= x >> 17; x ^= x << 5;
	return state->state = x;
}




#endif /* RANDOM_H_ */
