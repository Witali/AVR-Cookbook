# Interrupts and streaming

[Русская версия](realtime_ru.md)

## 1. Move data preparation from the ISR to the main loop

In LPC-sound-codec, the main loop reads the stream, builds a synthesis frame, and prepares increments for its subframes. The ISR switches to the prepared frame, generates the next sample, and updates PWM. In MicroMusic UM88, the fast ISR takes a ready PWM byte from the FIFO at the timer's cadence. Use measurements to decide where to divide the work, and finish writing the data before changing the index that publishes it.

Access to 16- or 32-bit values can tear even with `volatile`. LPC-sound-codec reads its 32-bit underrun counter inside `ATOMIC_BLOCK(ATOMIC_RESTORESTATE)`. Limit the critical section to the snapshot or publication and account for the time interrupts are masked.

## 2. Put the cursor in the low byte of the pointer

The UM88 buffer is aligned to 256 bytes and fits within one SRAM page. Load the high byte of its base address into XH and use the cursor directly in XL. After `LD ..., X+`, XL already holds the next cursor. The cursor wraps when it reaches the buffer capacity. In the isolated mono profile with a 192-byte buffer, this saved 8 bytes of code and reduced the IRQ from 52 to 49 cycles. The alignment added no padding in that memory layout. Measure padding again for a new layout, check the stack, and ensure the largest batch cannot step into the next page.

## 3. Preserve arithmetic flags on the common IRQ path

One UM88 revision uses `CPSE` to check for an empty ring and to detect the wrap point. It moves data with instructions that leave the arithmetic flags intact. The handwritten ISR can skip saving SREG on this path. One variant saves three registers in dedicated GPIOR registers in place of stack slots. Audit the entire ISR and ensure that interrupts cannot nest and that the ISR has exclusive use of those GPIOR registers.

In the ATtiny88 model, the previous mono IRQ took 44 cycles, the stack variant took 40, and the GPIOR variant took 34. The timings include interrupt acceptance, the vector, and RETI. Stereo with GPIOR took 38 cycles. The measurements start after any wait for the interrupted instruction to finish or for interrupts to be enabled. Also test the empty FIFO and wrap paths and verify the interrupted program state.

## 4. Bound the longest gap between FIFO publications

Size the FIFO for the worst-case producer delay. MicroMusic's Speex path split a long refill into smaller batches. In the isolated profile, the maximum interval between publications fell from 117,458 to 94,865 cycles and the image grew by 286 bytes. Measure FIFO headroom at full load with IRQs enabled, varied input packets, and actual SPI and timer rates. Include prefill, startup, reset, and slow paths even when the average throughput meets the deadline.

## 5. Prevent timer period error from accumulating

When the sample rate does not divide the timer clock, accumulate the fractional remainder and vary the integer intervals accordingly. The MicroMusic LPX AVR25 model observed sample-to-sample intervals of 1,991–2,009 cycles with no cumulative drift. That range is the jitter of the full system, including effects beyond the two rounded periods. Measure both long-term drift and individual sample jitter. Check when the timer accepts a compare update and what happens when an update is late.

[Source files and official references](sources.md)
