#include <stdint.h>

#define FIFO_SIZE 20u
#define BLOCK_SIZE 4u

#if !defined(VARIANT) || (VARIANT != 0 && VARIANT != 1)
#error "VARIANT must be 0 or 1"
#endif
#if FIFO_SIZE % BLOCK_SIZE
#error "Capacity must be a multiple of the block size"
#endif

/* The cursor is aligned and the entire block has room in the FIFO. */
static uint8_t store_block(volatile uint8_t *fifo, uint8_t cursor, uint8_t first)
{
#if VARIANT == 0
    for (uint8_t remaining = BLOCK_SIZE; remaining != 0; --remaining) {
        fifo[cursor] = first++;
        if (++cursor == FIFO_SIZE) {
            cursor = 0;
        }
    }
#else
    volatile uint8_t *block = fifo + cursor;
    for (uint8_t remaining = BLOCK_SIZE; remaining != 0; --remaining) {
        *block++ = first++;
    }
    cursor += BLOCK_SIZE;
    if (cursor == FIFO_SIZE) {
        cursor = 0;
    }
#endif
    return cursor;
}

#ifdef __AVR__
#include <avr/io.h>

static volatile uint8_t fifo[FIFO_SIZE];

int main(void)
{
    uint8_t cursor = 0;
    for (;;) {
        cursor = store_block(fifo, cursor, PINB);
    }
}
#else
#include <assert.h>

int main(void)
{
    for (unsigned cursor = 0; cursor < FIFO_SIZE; cursor += BLOCK_SIZE) {
        for (unsigned first = 0; first <= UINT8_MAX; ++first) {
            uint8_t fifo[FIFO_SIZE];
            for (unsigned i = 0; i < FIFO_SIZE; ++i) {
                fifo[i] = (uint8_t)(100u + i);
            }
            assert(store_block(fifo, (uint8_t)cursor, (uint8_t)first) ==
                   (cursor + BLOCK_SIZE) % FIFO_SIZE);
            for (unsigned i = 0; i < FIFO_SIZE; ++i) {
                uint8_t expected = (uint8_t)(100u + i);
                if (i >= cursor && i < cursor + BLOCK_SIZE) {
                    expected = (uint8_t)(first + i - cursor);
                }
                assert(fifo[i] == expected);
            }
        }
    }
    return 0;
}
#endif
