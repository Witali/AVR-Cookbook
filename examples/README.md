# AVR comparisons

[Русская версия](README_ru.md)

Run `python3 examples/measure.py` from the repository root. On Windows with the AVR tools on PATH, use `py -3 examples/measure.py`. The script uses `avr-gcc`, `avr-size`, and `avr-objdump`. It totals the sections from the standard linker scripts for ATmega328P and ATtiny85 and saves ELF files, link maps, and disassembly. Accesses to I/O registers give the compiler observable operations to retain. The comparisons run entirely on the host.

| Comparison | What to look for in the disassembly |
| --- | --- |
| `table-ram` → `table-flash` | The index is selected at runtime. The Flash variant loads through `LPM` and frees the SRAM occupied by the table. |
| `channels-runtime` → `channels-mono` | The mono build removes the channel count check and the write to OCR1AL. Both builds behave the same on the mono path. The runtime build also retains the stereo branch. |
| `compare-split` → `compare-chain` | Two comparisons use a single branch in the smaller routine. A low-byte mismatch takes longer. |
| `blend-two-products` → `blend-one-product` | The one-product fixed-point blend keeps the same mathematical floor for inputs in −16,000…16,000. [C source](blend.c) |
| `stride-multiply` → `stride-table` | For eight 23-byte records on ATtiny85, eight Flash bytes of offsets replace the runtime index multiplication. The second build drops `__mulhi3`. [C source](stride.c) |
| `fifo-wrap-byte` → `fifo-wrap-block` | For aligned four-byte writes into a 20-byte ring, the block variant advances the pointer within each block and checks wrap once. [C source](fifo-block.c) |
| `select-branch` → `select-skip` | A one-word `LDI` is conditionally skipped for a bit constrained to 0 or 1. [Assembly source](select-constant.S) |

The equality check receives both 16-bit operands in registers under the AVR-GCC ABI. If the low bytes differ, execution through RET takes 8 cycles in the split variant and 9 in the chained variant. If the low bytes match, the chained variant saves one cycle on either the equal or the high-byte mismatch path. These timings exclude CALL, entry to main, and RAM accesses.

The reference build uses AVR-GCC 7.3.0, `-Os`, section garbage collection, and each target's stock startup code.

| Comparison | MCU | Flash, bytes | Static SRAM, bytes |
| --- | --- | ---: | ---: |
| RAM table → Flash table | ATmega328P | 204 → 182 | 32 → 0 |
| Runtime mode → mono | ATmega328P | 178 → 140 | 2 → 0 |
| Two separate comparisons → CP/CPC | ATmega328P | 190 → 188 | 5 → 5 |
| Two-product blend → one product | ATmega328P | 324 → 294 | 6 → 6 |
| Runtime stride multiplication → Flash offsets | ATtiny85 | 128 → 102 | 185 → 185 |
| FIFO wrap per byte → wrap per four bytes | ATmega328P | 186 → 182 | 20 → 20 |
| Boolean branch → one-word skip | ATmega328P | 156 → 154 | 0 → 0 |

The figures are the loadable section sizes from the full test ELF files, including startup code. The first two pairs also change the initialization code for `.data`, which contributes to the Flash difference.

On the counted ATmega328P paths, `select_constant` takes 7 → 6 cycles for either selector, including RET and excluding CALL. The FIFO benchmark takes 68 → 42 cycles for a block without wrap and 68 → 44 at wrap. The FIFO count starts at the `PINB` read and ends before the next read. These are instruction counts from the linked disassembly. The test has no concurrent consumer.

## Check the standalone C recipes

The examples cover a signed dot product, fractional timer periods, and nonempty Flash ranges with an exclusive upper bound. The checks include an empty dot product, signed extrema, a full fractional timer cycle, and a write range that would overflow if computed by addition. [Recipe code](recipes.h) · [Boundary checks](check-recipes.c)

From the cookbook root, build and run the checks with a C compiler and a POSIX shell.

```sh
mkdir -p build
cc -std=c99 -Wall -Wextra -Werror examples/check-recipes.c -o build/check-recipes
./build/check-recipes
```

Both variants of the new C benchmarks also have host checks. They exercise signed blend endpoints and sampled inputs, all byte values for record selection, and every allowed FIFO cursor with every input byte.

```sh
for source in blend stride fifo-block; do
    for variant in 0 1; do
        cc -std=c99 -Wall -Wextra -Werror -DVARIANT="$variant" \
            "examples/$source.c" -o "build/$source-$variant"
        "./build/$source-$variant"
    done
done
```
