#include "recipes.h"

#include <assert.h>
#include <stddef.h>

static void check_dot(void)
{
    int8_t left[255];
    int8_t right[255];
    for (size_t i = 0; i < sizeof left; ++i) {
        left[i] = INT8_MIN;
        right[i] = INT8_MIN;
    }

    assert(dot_i8(NULL, NULL, 0) == 0);
    assert(dot_i8(left, right, 1) == 16384);
    assert(dot_i8(left, right, 255) == 4177920L);

    for (size_t i = 0; i < sizeof right; ++i) {
        right[i] = INT8_MAX;
    }
    assert(dot_i8(left, right, 255) == -4145280L);
}

static void check_period(void)
{
    struct fractional_period half = {12, 1, 2, 0};
    assert(period_next_ticks(&half) == 12);
    assert(period_next_ticks(&half) == 13);
    assert(half.phase == 0);

    struct fractional_period integer = {UINT16_MAX, 0, 1, 0};
    assert(period_next_ticks(&integer) == UINT16_MAX);

    struct fractional_period large = {12, 65534u, 65535u, 0};
    uint32_t total = 0;
    for (uint32_t n = 1; n <= large.denominator; ++n) {
        total += period_next_ticks(&large);
        assert(total == n * 12u + n * 65534u / 65535u);
    }
    assert(large.phase == 0);
}

static void check_flash_span(void)
{
    assert(flash_span_inside(0x100u, 0x200u, 0x100u, 0x300u));
    assert(flash_span_inside(0x2ffu, 1, 0x100u, 0x300u));
    assert(!flash_span_inside(0x0ffu, 1, 0x100u, 0x300u));
    assert(!flash_span_inside(0x300u, 1, 0x100u, 0x300u));
    assert(!flash_span_inside(0x2ffu, 2, 0x100u, 0x300u));
    assert(!flash_span_inside(0x100u, 0, 0x100u, 0x300u));
    assert(!flash_span_inside(UINT32_MAX - 15u, 32, 0, UINT32_MAX));
}

int main(void)
{
    check_dot();
    check_period();
    check_flash_span();
    return 0;
}
