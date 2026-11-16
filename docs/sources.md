# Sources and verification scope

[Русская версия](sources_ru.md)

Paths in the tables are relative to the source projects. To inspect a file, run `git show <revision>:<path>` in the corresponding clone. The cookbook contains original explanations and short examples. Firmware and project source files are available in their respective repositories.

| Project | Revision | Devices in this cookbook |
| --- | --- | --- |
| MicroMusic | `1fe7048d7b29` | ATtiny13A, ATtiny88, ATmega328P |
| optiboot | `e3f93466abd0` | ATmega328P/PB, ATtiny84/85, ATtiny841/1634 |
| MicroNucleus, nested repository `src/micronucleus` | `b2ae7c84c490` | ATtiny88 |
| LPC-sound-codec | `360af1460785` | ATmega328P / Arduino Nano |

## MicroMusic

| Topic | File | Evidence |
| --- | --- | --- |
| Assembly overview, multiply chain, and ABI | `docs/avr-asm-cookbook/README.md` | Size, cycles, PCM comparisons, and modeled interrupts |
| FIFO within a single page | `docs/avr-asm-cookbook/um88-page-fifo.md` | Isolated size measurements and interrupt model |
| Flags and GPIOR | `docs/avr-asm-cookbook/um88-flag-neutral-irq.md` | Cycles, interrupts, and boundary cases in the model |
| Mono and stereo modes | `docs/avr-asm-cookbook/um88-configured-channels.md` | Build and model results, including missed deadlines in C stereo |
| UM88 implementation | `ATtiny88/Music/universal/platform.c`, `ATtiny88/Music/universal/irq-avr.S`, `ATtiny88/Music/universal/storage.h` | Platform, ISR, and storage interface source |
| Arithmetic kernel selection | `Codecs/LPX/01_ACTIVE_Q0_CODEC/src/Lpx4Math.h`, `docs/avr-asm-cookbook/lpx-multiply-selection.md` | Disassembly for AVR5 and AVR25 |
| Specialized DSP recipes | [Topic index](micromusic-index.md) | Each source note describes its profile and tests |

## optiboot

| Topic | File | Evidence |
| --- | --- | --- |
| C build settings and optional features | `optiboot/bootloaders/optiboot/Makefile`, `optiboot/bootloaders/optiboot/optiboot.c` | Source code |
| 368/358/356/350 bytes, length checks, and shared tail | `docs/atmega328p/history/03_358_350_size_optimization/README.md` | Builds and instruction-model comparisons |
| Current Mini, Middle, and Full builds | `docs/mini/README.md`, `optiboot/bootloaders/optiboot/optiboot_mini.S` | Profile requirements and implementation |
| Virtual Boot layout, boundaries, and build matrix | `docs/mini/VIRTUAL_BOOT.md`, `docs/mini/INTEGRATION_2026-11-10.md` | GCC and LLVM builds, command and memory models |
| Middle and Full on a Nano board | `docs/mini/HARDWARE_MIDDLE_ATMEGA328PB_2026-11-04.md`, `docs/mini/HARDWARE_FULL_ATMEGA328PB_2026-11-04.md` | Hardware verification of specific images on ATmega328PB |

## MicroNucleus

Prefix the paths below with `experimental/attiny88/`.

| Topic | File | Evidence |
| --- | --- | --- |
| ATtiny88 history and differences between development lines | `README.md` | Image summary and verification levels |
| Combined CRC and copy, bit I/O, STD, and alignment | `variants/strict_usb_optimized/README.md`, `variants/strict_usb_optimized/micronucleus_t88_optimized.S` | Builds and model for the 1,262-byte version with LED support disabled |
| USB, Flash, and EEPROM on a board | `variants/strict_usb_optimized/HARDWARE_TEST.md` | Hardware verification of the earlier 1,274-byte image |
| SETUP state, CPC chains, polling, and rejected reductions | `flash_only/docs/SIZE_REDUCTIONS.md`, `flash_only/source/micronucleus_t88_compact.S` | Separate development line, model results, and open USB qualification |
| Problem in the 1,024-byte prototype | `flash_only/README.md` | Premature SPM in the model |
| 512/32/16/0 bytes for CRC16 tables | `block64/prototypes/streaming_crc16/CRC_TABLE_TRADEOFFS.md` | Isolated kernels and receive cycle budget |

## LPC-sound-codec

| Project | File | Topic |
| --- | --- | --- |
| LPC-sound-codec | `arduino/Lpc2AvrDecoder/src/Lpc2AvrDecoder.cpp` | Flash tables, frame preparation, timers, and an atomic counter snapshot |
| LPC-sound-codec | `arduino/Lpc2AvrDecoder/examples/LPC2_PROGMEM_Player/LPC2_PROGMEM_Player.ino` | Reading a stream one byte at a time from program memory |

## AVR documentation

- Microchip. [ATmega328P registers and BOOTSZ table](https://ww1.microchip.com/downloads/en/DeviceDoc/ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061A.pdf) · [ATtiny88 timers and I/O](https://ww1.microchip.com/downloads/en/DeviceDoc/doc8008.pdf) · [AVR instruction set summary](https://onlinedocs.microchip.com/oxy/GUID-06C36746-0136-4691-8940-69BFD41FCC56-en-US-1/GUID-64F3D141-2594-4755-9103-550108045801.html)
- GCC. [AVR options and target macros in GCC 7.3](https://gcc.gnu.org/onlinedocs/gcc-7.3.0/gcc/AVR-Options.html)
- AVR-LibC. [Program space](https://avrdudes.github.io/avr-libc/avr-libc-user-manual/group__avr__pgmspace.html) · [Memory sections](https://avrdudes.github.io/avr-libc/avr-libc-user-manual/mem_sections.html) · [ABI and register usage](https://avrdudes.github.io/avr-libc/avr-libc-user-manual/FAQ.html#faq_reg_usage) · [Atomic blocks](https://avrdudes.github.io/avr-libc/avr-libc-user-manual/group__util__atomic.html)
