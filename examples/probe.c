#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdint.h>

#if PROBE == 1

#if VARIANT
#define STORAGE PROGMEM
#else
#define STORAGE
#endif

static const uint8_t curve[32] STORAGE = {
    0, 1, 2, 3, 5, 7, 10, 13, 17, 21, 26, 31, 37, 43, 50, 57,
    65, 73, 82, 91, 101, 111, 122, 133, 145, 157, 170, 183,
    197, 211, 226, 241
};

int main(void)
{
    for (;;) {
        uint8_t index = PINB & 31u;
#if VARIANT
        OCR1BL = pgm_read_byte(&curve[index]);
#else
        OCR1BL = curve[index];
#endif
    }
}

#elif PROBE == 2

#if !VARIANT
volatile uint8_t channel_count = 1;
#endif

int main(void)
{
    for (;;) {
        uint8_t sample = PINB;
        OCR1BL = sample;
#if !VARIANT
        if (channel_count == 2) {
            OCR1AL = UINT8_MAX - sample;
        }
#endif
    }
}

#elif PROBE == 3

volatile uint16_t a;
volatile uint16_t b;
volatile uint8_t result;
uint8_t words_equal(uint16_t left, uint16_t right);

int main(void)
{
    for (;;) {
        result = words_equal(a, b);
    }
}

#else
#error "Select a PROBE in measure.py"
#endif
