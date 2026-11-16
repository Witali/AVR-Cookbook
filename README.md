# AVR-Cookbook

[Русская версия](README_ru.md)

Practical optimizations for classic 8-bit AVR microcontrollers, drawn from the source code and measurements of MicroMusic, optiboot, MicroNucleus, and LPC-sound-codec. Each recipe explains what to measure and which assumptions to check when applying it.

| When you need to… | Start with… |
| --- | --- |
| Find where memory and CPU time go | [Measurement guide](docs/measurement.md) for ELF files, link maps, listings, stack usage, and worst-case paths |
| Save SRAM or Flash, or reduce EEPROM wear | [Memory and configuration](docs/memory.md) for Flash tables, build-time specialization, and circular journals |
| Save a few instructions or cycles | [AVR assembly](docs/assembly.md) for flags, registers, pointers, branches, and the ABI |
| Reduce IRQ latency or prevent FIFO underruns | [Interrupts and streaming](docs/realtime.md) for timers, FIFO sizing, prefill, and latency bounds |
| Fit DSP into the cycle budget | [Integer arithmetic](docs/arithmetic.md) for bounded ranges, MAC, reused products, and offline computation |
| Fit a bootloader into its Flash region | [Bootloaders and protocols](docs/bootloaders.md) for shared code, placement, entry conditions, and race tests |

The source index maps recipes to project files and official references. The MicroMusic index covers more specialized LPX, Speex, and synthesis experiments. [Source index](docs/sources.md) · [MicroMusic audio index](docs/micromusic-index.md)

To run the three comparisons for ATmega328P, install Python 3.10+, AVR-GCC, AVR-LibC, and AVR binutils, then run the measurement script.

```sh
python3 examples/measure.py
```

The script builds both variants of each example on the host and saves the ELF files, link maps, and disassembly in `build/measure/`. [What the examples compare](examples/README.md)

Measurements apply to the builds described in each chapter. Build verification covers the firmware image and its size. Model verification covers instruction execution or peripheral simulation. Board verification applies to the specific images and conditions documented in the corresponding reports.
