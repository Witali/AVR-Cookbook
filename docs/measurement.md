# Measurement guide

[Русская версия](measurement_ru.md)

Record `-mmcu`, the clock frequency, compiler, linker, and header versions, the load address, and the build features. Compare images with the same behavior. Treat changes to the UART, protocol, command set, or arithmetic precision as separate changes. When comparing toolchains, record the exact build matrix and compare the generated images.

## Build and inspect the linked ELF

```sh
avr-gcc -mmcu=atmega328p -Os -ffunction-sections -fdata-sections \
  -Wl,--gc-sections,-Map=firmware.map -o firmware.elf main.c
avr-size -A firmware.elf
avr-objdump -h -d firmware.elf > firmware.lst
avr-nm -S --size-sort firmware.elf
```

- Count Flash sections that actually load into program memory. On a typical classic AVR, these include code, the initialization image for `.data`, and any additional Flash sections. `.data` also occupies SRAM, as do `.bss` and `.noinit`. Count EEPROM separately. Check the LMA and VMA and the last occupied address in the link map. Gaps and alignment can affect placement even when the sum of section sizes falls.
- Compare `-Os` with other optimization settings on the same application. Experiment with `-fno-split-wide-types`, LTO, and `-mrelax` one at a time. Try `--gc-sections` with separate function and data sections. `-mrelax` can shorten `CALL` and `JMP` at link time. For custom tables and vectors, check which sections the linker treats as roots and which the linker script marks with `KEEP`. [Toolchain references](sources.md)
- Count worst-case cycles using instructions **from the linked ELF**. Include taken branches, skips over one- and two-word instructions, calls, and interrupts. Account separately for the time to finish the current instruction and for periods with interrupts disabled.
- Add static data, peak stack usage including ISRs, alignment padding, and any heap usage. The `-fstack-usage` report helps with C. To account for nested calls, handwritten assembly, and interrupt paths in a complex image, observe the minimum stack pointer in the model.

## Check equivalent behavior

Start with boundary cases such as zero, the maximum value, page crossings, signed extrema, short and malformed packets, retries, and resets. For arithmetic, compare against an independent implementation with wide intermediates. For timing changes, drive the system under heavy load and interrupt critical code at different points. When testing a stream, check output samples and FIFO underruns separately.

For hardware interfaces, measure the intervals at the pins and vary the input phase. USB electrical behavior needs its own measurements beyond simulating correct bits at exactly 16 MHz. For every result, record the MCU, configuration, metric, baseline, and verification method.
