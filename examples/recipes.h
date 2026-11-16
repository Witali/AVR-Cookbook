#ifndef AVR_COOKBOOK_RECIPES_H
#define AVR_COOKBOOK_RECIPES_H

#include <stdbool.h>
#include <stdint.h>

/* Up to 255 signed byte products fit in an int32_t accumulator. */
static inline int32_t dot_i8(const int8_t *left, const int8_t *right, uint8_t count)
{
    if (count == 0) {
        return 0;
    }

    int32_t sum = (int16_t)left[0] * right[0];
    for (uint8_t i = 1; i < count; ++i) {
        sum += (int16_t)left[i] * right[i];
    }
    return sum;
}

struct fractional_period {
    uint16_t whole_ticks;
    uint16_t remainder;
    uint16_t denominator;
    uint16_t phase;
};

/* Start with phase = 0, with remainder < denominator.
 * When remainder is nonzero, whole_ticks + 1 must fit in uint16_t.
 */
static inline uint16_t period_next_ticks(struct fractional_period *period)
{
    const uint16_t gap = period->denominator - period->remainder;
    if (period->phase >= gap) {
        period->phase -= gap;
        return period->whole_ticks + 1u;
    }

    period->phase += period->remainder;
    return period->whole_ticks;
}

/* All arguments are byte units. The protected limit is exclusive. */
static inline bool flash_span_inside(uint32_t address, uint32_t length,
                                     uint32_t first, uint32_t limit)
{
    return address >= first && address < limit &&
           length != 0 && length <= limit - address;
}

#endif
