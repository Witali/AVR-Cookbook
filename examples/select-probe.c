#include <avr/io.h>
#include <stdint.h>

uint8_t select_constant(uint8_t fallback, uint8_t selector);

int main(void)
{
    for (;;) {
        // Keep both builds on the same runtime 0/1 input domain.
        OCR1BL = select_constant(0x39, PINB & 1u);
    }
}
