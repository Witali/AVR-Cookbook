# AVR assembly

[Русская версия](assembly_ru.md)

Inspect the linked code before applying these techniques. Keep the C source if AVR-GCC already emits the same sequence. Check the instruction set and timing for your AVR core.

## 1. Use both an instruction's result and its flags

In a historical ATmega328P optiboot build, `LSR` converts a byte count to a word count. It sets C if the original count is odd and Z if the result is zero. Reusing the flags saved 2 bytes in the parity and range checks. In MicroNucleus a counter that reaches zero after `SBIW` can clear another register pair through `MOVW`, provided the counter reaches zero on every path that uses it. Preserve the flag between the instruction that sets it and the instruction that reads it.

## 2. Chain comparisons with CPC

For an unsigned 16-bit comparison, compare the low bytes first. `CPC` takes the carry from `CP` and leaves Z set only if both the earlier bytes and the current byte match.

```asm
; a = r25:r24, b = r23:r22
cp   r24, r22
cpc  r25, r23
breq equal
; BRLO can test a < b here while C is intact
```

In Flash-only MicroNucleus, chaining the length, address, and CRC checks for a token saved 4 bytes and two cycles on a valid token. The change required retuning the receive delay. Start-of-packet (SOP) boundary tests covered the affected path. [Two implementations of the equality check](../examples/README.md)

## 3. Reuse pointers and keep state in registers

On cores that support these forms, MicroMusic reads sequential data with `LD Rd, X+` or `LPM Rd, Z+`. It keeps the MAC accumulator in registers and uses `MOVW` to copy both bytes between even-numbered register pairs. In MicroNucleus, replacing `STS` with `STD Z+20` saved 2 bytes and kept the store at two cycles. Z already pointed to the context on every path that reached the store. If the base needs a separate load, recalculate size and latency. `STD` with a positive displacement uses Y or Z and has a limited displacement range.

## 4. Reduce branches while accounting for every path

Place a simple case next to its condition and invert the conditional branch to remove an extra jump. MicroNucleus did this for GET_CONFIGURATION. MicroMusic counts down with `DEC/BRNE`. With counts from 0 to 255, handle zero separately to avoid running 256 iterations. Branch distance, taken paths, and the size of a skipped instruction can all affect the gain.

## 5. Use direct bit instructions with suitable registers

On classic AVR, `SBI/CBI/SBIC/SBIS` address the lowest 32 locations in **I/O space**. MicroNucleus used `SBI` on PCIF2 in PCIFR on ATtiny88. Three replacements saved 6 bytes while keeping each operation at two cycles. Check the device datasheet before replacing a mask write with a read-modify-write on any write-one-to-clear register. Verify the instruction's effect on adjacent flags.

## 6. Respect the ABI and preserve interrupted state

Under the classic AVR-GCC ABI, `MUL` writes to `r1:r0`. Restore zero in r1 before returning to or calling C. An interrupt can arrive in the middle of an assembly routine with a nonzero r1. When a compiler-generated ISR uses the zero register, it saves, clears, and restores it. A handwritten ISR must preserve the interrupted state. Routines called from C must also preserve any callee-saved registers they modify. For inline assembly, describe early-clobber outputs and memory effects in the constraints and clobber list. AVR Reduced Core uses a different ABI.

MicroMusic's Speex code removed redundant clears of r1 inside an internal chain of multiplies and kept the clear before the next instruction that requires zero. On ATmega328P this saved 12 bytes of Flash. The three kernel paths went from 240/330/420 to 238/327/416 cycles, including RET and excluding CALL. The model also injected interrupts with r1 nonzero. Restore r1 at the C boundary.

[Source files and official references](sources.md)
