# AVR assembly

[Русская версия](assembly_ru.md)

Inspect the linked code before applying these techniques. Keep the C source if AVR-GCC already emits the same sequence. Check the instruction set and timing for your AVR core.

## 1. Use both an instruction's result and its flags

`LSR` converts an 8-bit byte count to a word count. It sets C if the original count is odd and Z if the result is zero, so the same instruction can feed parity and length checks. A counter that reaches zero after `SBIW` can clear another register pair through `MOVW`, provided the counter reaches zero on every path that uses it. Preserve the flag between the instruction that sets it and the instruction that reads it.

## 2. Chain comparisons with CPC

For an unsigned 16-bit comparison, compare the low bytes first. `CPC` takes the carry from `CP` and leaves Z set only if both the earlier bytes and the current byte match.

```asm
; a = r25:r24, b = r23:r22
cp   r24, r22
cpc  r25, r23
breq equal
; BRLO can test a < b here while C is intact
```

Measure both equal and unequal inputs, since chaining can make an early mismatch slower. In a receiver with fixed sampling windows, also check timing from the start of the packet after changing the comparison path. [Two implementations of the equality check](../examples/README.md)

## 3. Reuse pointers and keep state in registers

Read sequential data with `LD Rd, X+` or `LPM Rd, Z+` on cores that support these forms. Keep a MAC accumulator in registers and use `MOVW` to copy both bytes between register pairs whose low registers are even-numbered. For a field at offset 20, `STD Z+20` can be shorter than `STS` if Z already points to the structure on every path. If the base needs a separate load, recalculate size and latency. `STD` with a displacement uses Y or Z and has a limited displacement range.

## 4. Reduce branches while accounting for every path

Place a simple case next to its condition and invert the conditional branch to remove an extra jump. For loops, count down with `DEC/BRNE`. With counts from 0 to 255, handle zero separately to avoid running 256 iterations. Branch distance, taken paths, and the size of a skipped instruction can all affect the gain.

## 5. Use direct bit instructions with suitable registers

On classic AVR, `SBI/CBI/SBIC/SBIS` address the lowest 32 locations in **I/O space**. Use them when the register's address and semantics allow it. Check the device datasheet before replacing a mask write with a read-modify-write on any write-one-to-clear register. Verify the instruction's effect on adjacent flags.

## 6. Respect the ABI and preserve interrupted state

Under the classic AVR-GCC ABI, `MUL` writes to `r1:r0`. Restore zero in r1 before returning to or calling C. An interrupt can arrive in the middle of an assembly routine with a nonzero r1. When a compiler-generated ISR uses the zero register, it saves, clears, and restores it. A handwritten ISR must preserve the interrupted state. Routines called from C must also preserve any callee-saved registers they modify. For inline assembly, describe early-clobber outputs and memory effects in the constraints and clobber list. AVR Reduced Core uses a different ABI.

In an internal chain of multiplies, defer redundant clears of r1 when no intervening instruction needs it to be zero. Restore r1 before the next consumer of zero and at the C boundary. Exercise interrupts at points where r1 is nonzero and verify that the ISR restores the interrupted value.

[Source files and official references](sources.md)
