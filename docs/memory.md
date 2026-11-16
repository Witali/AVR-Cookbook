# Memory and configuration

[Русская версия](memory_ru.md)

## 1. Keep constant tables and strings in Flash

Place constant tables in `PROGMEM` and read elements with the accessor for their width, such as `pgm_read_byte` or `pgm_read_word`. A 257-element table of 16-bit values contains 514 bytes of data. A common read interface can handle program memory and other storage. Read strings from Flash one byte at a time when sending them directly to a UART.

```c
#include <avr/pgmspace.h>
#include <stdint.h>

static const uint8_t steps[] PROGMEM = {3, 5, 8, 13};
uint8_t step(uint8_t i) { return pgm_read_byte(&steps[i & 3u]); }
```

On ATmega328P, select the address space explicitly. A plain dereference of a pointer to a `PROGMEM` object will not generate `LPM`. On a classic AVR with data above 64 KiB, check the address in the link map and use far access with the full byte address where needed. Assembly operands for `LPM` use byte addresses. Check branch addresses and relocations separately. [Flash versus SRAM example](../examples/README.md)

## 2. Exclude unused modes at build time

Select variants with build-time options so that unused code can be left out. For example, a mono build can omit right-channel mixing, stereo interpolation, and writes to an unused compare register. Other options can select the UART implementation, command set, or LED support. Compare time and size on shared features and verify each configuration. In the example, inspect the disassembly to check that the mono build has no writes to OCR1AL. [Build comparison](../examples/README.md)

## 3. Reduce EEPROM writes

Distribute frequent settings updates across a ring of EEPROM slots. On Arduino AVR, `EEPROM.update()` writes a byte only if its value changes. For records spanning several bytes, provide a way to recover the most recent complete record. Test power loss after every operation, sequence number wraparound, and the transition from the last slot back to the first.

## 4. Reuse storage according to data lifetime

Reuse storage when the lifetimes of its contents cannot overlap. For example, a packet buffer can be shared between receive and response preparation if ownership changes only after the previous reader has finished. A writer can skip clearing an inactive buffer if it initializes every byte the reader can see before publishing it. Exercise cold starts with randomized RAM and registers, resets, and packets that arrive early. Zero-filled simulator state can hide initialization bugs.

## 5. Look up fixed-stride record offsets in Flash

On an AVR without hardware multiplication, indexing a record with an awkward stride can bring in a software multiply routine. For a small, fixed record count, generate a Flash table of byte offsets using `index * sizeof(record)` at compile time. Read the offset with `pgm_read_byte` and add it to the array base.

Check the index before reading the table and prove at compile time that the largest offset fits the table entry. A wider offset needs a wider table and read. Compare the linked image, since the table and its access code can cost more than multiplication on another target. Check the first and last valid elements. [Standalone example](../examples/stride.c) · [Build measurements](../examples/README.md)

## 6. Skip a second clear of fresh .bss state

With the standard AVR startup, objects in `.bss` are already zero when `main` starts. An initializer that runs once can set just the nonzero fields. Reinitialization after use still needs an explicit clear or assignment. Check that every entry path runs the startup clear and that nothing writes the state before initialization. Objects in `.noinit`, code running before the startup clear, and entry paths that bypass the startup code need separate handling.

Compare the linked startup path to see whether removing the clear saves any instructions. Check both cold startup and any in-place restart using nonzero state. [AVR startup sections](sources.md)

## 7. Compute only the intermediate values you need

When a later stage consumes selected entries of a large intermediate array, try computing those entries on demand. For example, a filter can evaluate only the frequency bins its next stage reads. Keep operation order and rounding compatible with a reference where output must be exact.

Recomputation can reduce working SRAM and increase execution time. Measure static SRAM and peak stack together. Include interrupts and the slowest data path when testing deadlines, since a small persistent state can still leave too little room for temporary data or too few cycles for the next input.

[Source files and official references](sources.md)
