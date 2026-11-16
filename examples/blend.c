#include <stdint.h>

#ifndef VARIANT
#define VARIANT 0
#endif

#if VARIANT != 0 && VARIANT != 1
#error "VARIANT must be 0 (two products) or 1 (one product)"
#endif

/* AVR-GCC uses an arithmetic right shift for signed values. */
static int16_t floor_q14(int32_t numerator)
{
#ifdef __AVR__
    return (int16_t)(numerator >> 14);
#else
    if (numerator >= 0) {
        return (int16_t)(numerator / 16384);
    }
    return (int16_t)(-1 - ((-1 - numerator) / 16384));
#endif
}

/* Both inputs are in [-16000, 16000]. The difference fits in int16_t. */
#ifdef __AVR__
__attribute__((noinline))
#endif
static int16_t blend_q14(int16_t previous, int16_t input)
{
#if VARIANT == 0
    const int32_t weighted =
        (int32_t)previous * 13383 + (int32_t)input * 3001;
    return floor_q14(weighted + 8192);
#else
    const int16_t difference = (int16_t)(input - previous);
    return (int16_t)(previous + floor_q14((int32_t)difference * 3001 + 8192));
#endif
}

#ifdef __AVR__
static volatile int16_t previous_sample = -16000;
static volatile int16_t input_sample = 16000;
static volatile int16_t output_sample;

int main(void)
{
    for (;;) {
        output_sample = blend_q14(previous_sample, input_sample);
    }
}
#else
#include <assert.h>
#include <stddef.h>

/* Independent 64-bit numerator and adjustment after division toward zero. */
static int16_t reference(int16_t previous, int16_t input)
{
    const int64_t sum = (int64_t)previous * (16384 - 3001) +
                        (int64_t)input * 3001 + 8192;
    return (int16_t)(sum / 16384 - (sum % 16384 < 0));
}

int main(void)
{
    static const int16_t corners[] = {
        -16000, -15999, -8192, -1, 0, 1, 8192, 15999, 16000
    };
    for (size_t i = 0; i < sizeof corners / sizeof corners[0]; ++i) {
        for (size_t j = 0; j < sizeof corners / sizeof corners[0]; ++j) {
            assert(blend_q14(corners[i], corners[j]) ==
                   reference(corners[i], corners[j]));
        }
    }
    for (int32_t previous = -16000; previous <= 16000; previous += 137) {
        for (int32_t input = -16000; input <= 16000; input += 251) {
            assert(blend_q14((int16_t)previous, (int16_t)input) ==
                   reference((int16_t)previous, (int16_t)input));
        }
    }
    return 0;
}
#endif
