#include <stddef.h>
#include <stdint.h>

#ifdef __AVR__
#include <avr/io.h>
#include <avr/pgmspace.h>
#else
#include <assert.h>
#endif

#if !defined(VARIANT) || (VARIANT != 0 && VARIANT != 1)
#error "Set VARIANT=0 for array indexing or VARIANT=1 for Flash offsets"
#endif

enum { RECORD_COUNT = 8 };

struct record {
    uint8_t status;
    uint8_t payload[22];
};

static volatile struct record records[RECORD_COUNT];
#ifdef __AVR__
static volatile uint8_t status_sink;
#endif

#if VARIANT == 1
typedef char offsets_fit_in_a_byte[
    (RECORD_COUNT - 1u) * sizeof(struct record) <= UINT8_MAX ? 1 : -1];

#define RECORD_OFFSET(i) ((uint8_t)((i) * sizeof(struct record)))
static const uint8_t offsets[RECORD_COUNT]
#ifdef __AVR__
    PROGMEM
#endif
    = {
        RECORD_OFFSET(0), RECORD_OFFSET(1), RECORD_OFFSET(2), RECORD_OFFSET(3),
        RECORD_OFFSET(4), RECORD_OFFSET(5), RECORD_OFFSET(6), RECORD_OFFSET(7)
    };
#undef RECORD_OFFSET
#endif

static uint8_t read_status(uint8_t input)
{
    const uint8_t row = input & (RECORD_COUNT - 1u);
#if VARIANT == 0
    return records[row].status;
#else
#ifdef __AVR__
    const uint8_t offset = pgm_read_byte(&offsets[row]);
#else
    const uint8_t offset = offsets[row];
#endif
    const volatile unsigned char *bytes = (const volatile unsigned char *)&records;
    return bytes[offset + offsetof(struct record, status)];
#endif
}

int main(void)
{
#ifdef __AVR__
    for (;;) {
        status_sink = read_status(PINB);
    }
#else
    for (unsigned int row = 0; row < RECORD_COUNT; ++row) {
        records[row].status = (uint8_t)(11u + 29u * row);
        records[row].payload[0] = 255u;
    }
    for (unsigned int input = 0; input <= UINT8_MAX; ++input) {
        const uint8_t expected = (uint8_t)(11u + 29u * (input % RECORD_COUNT));
        assert(read_status((uint8_t)input) == expected);
    }
    return 0;
#endif
}
