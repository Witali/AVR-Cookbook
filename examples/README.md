# ATmega328P comparisons

[Русская версия](README_ru.md)

Run `python3 examples/measure.py` from the repository root. On Windows with the AVR tools on PATH, use `py -3 examples/measure.py`. The script uses `avr-gcc`, `avr-size`, and `avr-objdump`. It totals the sections from the standard ATmega328P link layout and saves ELF files, link maps, and disassembly. Accesses to I/O registers give the compiler observable operations to retain. The comparisons can be run entirely on the host.

| Comparison | What to look for in the disassembly |
| --- | --- |
| `table-ram` → `table-flash` | The index is selected at runtime. The Flash variant loads through `LPM` and frees the SRAM occupied by the table. |
| `channels-runtime` → `channels-mono` | The mono build removes the channel count check and the write to OCR1AL. Both builds behave the same on the mono path. The runtime build also retains the stereo branch. |
| `compare-split` → `compare-chain` | Two comparisons use a single branch in the smaller routine. A low-byte mismatch takes longer. |

The equality check receives both 16-bit operands in registers under the AVR-GCC ABI. If the low bytes differ, execution through RET takes 8 cycles in the split variant and 9 in the chained variant. If the low bytes match, the chained variant saves one cycle on either the equal or the high-byte mismatch path. These timings exclude CALL, entry to main, and RAM accesses.

The reference build uses AVR-GCC 7.3.0, `-Os`, and the standard ATmega328P startup code.

| Comparison | Flash, bytes | Static SRAM, bytes |
| --- | ---: | ---: |
| RAM table → Flash table | 204 → 182 | 32 → 0 |
| Runtime mode → mono | 178 → 140 | 2 → 0 |
| Two separate comparisons → CP/CPC | 190 → 188 | 5 → 5 |

The figures are the loadable section sizes from the full test ELF files, including startup code. The first two pairs also change the initialization code for `.data`, which contributes to the Flash difference.
