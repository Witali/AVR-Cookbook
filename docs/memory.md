# Memory and configuration

[Русская версия](memory_ru.md)

## 1. Keep constant tables and strings in Flash

LPC-sound-codec stores a 257-element signed 16-bit cosine table in `PROGMEM`. The decoder reads individual words with `pgm_read_word`. The table data itself occupies 514 bytes. MicroMusic uses a common read interface for program memory and other storage. Strings can likewise be read from Flash and sent directly to the UART one byte at a time.

```c
#include <avr/pgmspace.h>
#include <stdint.h>

static const uint8_t steps[] PROGMEM = {3, 5, 8, 13};
uint8_t step(uint8_t i) { return pgm_read_byte(&steps[i & 3u]); }
```

On ATmega328P, select the address space explicitly. A plain dereference of a pointer to a `PROGMEM` object will not generate `LPM`. On a classic AVR with data above 64 KiB, check the address in the link map and use far access with the full byte address where needed. Assembly operands for `LPM` use byte addresses. Check branch addresses and relocations separately. [Flash versus SRAM example](../examples/README.md)

## 2. Exclude unused modes at build time

The MicroMusic ATtiny88 mono profile omits right-channel mixing, stereo interpolation, and writes to OCR1A. The optiboot profile selects its command set and UART implementation. MicroNucleus can build a variant with LED support disabled. Make mutually exclusive profiles explicit. When removing a channel or command, compare time and size on the shared features and verify the behavior required of each configuration. The MicroMusic mono test detects even writes that would leave the value in OCR1A unchanged.

## 3. Reduce EEPROM writes

Distribute frequent settings updates across a ring of EEPROM slots. On Arduino AVR, `EEPROM.update()` writes a byte only if its value changes. For records spanning several bytes, provide a way to recover the most recent complete record. Test power loss after every operation, sequence number wraparound, and the transition from the last slot back to the first.

## 4. Reuse storage according to data lifetime

The strict MicroNucleus USB bootloader reuses persistent state and counters when building responses. Its Flash-only experiment leaves inactive packet buffers uninitialized until they are ready to publish. This can save code or SRAM when readers can see a buffer only after the writer has finished. Initialize everything a reader can already access. Exercise cold starts with randomized RAM and registers, resets, and packets that arrive early. Zero-filled simulator state can hide initialization bugs.

[Source files and official references](sources.md)
