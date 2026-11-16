# AVR references

[Русская версия](sources_ru.md)

Use the datasheet for the exact MCU when checking registers, Flash and EEPROM geometry, clocking, and instruction timing.

- Microchip. [ATmega328P registers and BOOTSZ table](https://ww1.microchip.com/downloads/en/DeviceDoc/ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061A.pdf) · [ATtiny88 timers and I/O](https://ww1.microchip.com/downloads/en/DeviceDoc/doc8008.pdf) · [AVR instruction set summary](https://onlinedocs.microchip.com/oxy/GUID-06C36746-0136-4691-8940-69BFD41FCC56-en-US-1/GUID-64F3D141-2594-4755-9103-550108045801.html)
- GCC. [AVR options and target macros in GCC 7.3](https://gcc.gnu.org/onlinedocs/gcc-7.3.0/gcc/AVR-Options.html)
- AVR-LibC. [Program space](https://avrdudes.github.io/avr-libc/avr-libc-user-manual/group__avr__pgmspace.html) · [Memory sections](https://avrdudes.github.io/avr-libc/avr-libc-user-manual/mem_sections.html) · [ABI and register usage](https://avrdudes.github.io/avr-libc/avr-libc-user-manual/FAQ.html#faq_reg_usage) · [Atomic blocks](https://avrdudes.github.io/avr-libc/avr-libc-user-manual/group__util__atomic.html)

The cookbook examples include the target, build flags, and results. Inspect the linked instructions to verify how each change affects size or execution. [Measurement guide](measurement.md) · [Reproducible comparisons](../examples/README.md)
