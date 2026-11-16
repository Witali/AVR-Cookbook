# Interrupts and streaming

[Русская версия](realtime_ru.md)

## 1. Move data preparation from the ISR to the main loop

Read the stream, decode frames, or prepare interpolation increments in the main loop. At each timer tick, the ISR can switch to a prepared frame or take a ready PWM value from a FIFO. Use the worst-case cycle budget to decide where to divide the work. Finish writing the data before changing the index that publishes it.

Access to 16- or 32-bit values can tear even with `volatile`. Read a 32-bit counter updated by the ISR inside `ATOMIC_BLOCK(ATOMIC_RESTORESTATE)`. Limit the critical section to the snapshot or publication and account for the time interrupts are masked.

## 2. Put the cursor in the low byte of the pointer

Align the ring buffer to 256 bytes and keep it within one SRAM page. Load the high byte of its base address into XH and use the cursor directly in XL. After `LD ..., X+`, XL already holds the next cursor if the pointer has not crossed the page. Wrap the cursor at the buffer capacity. Account for linker padding and stack space, and test the largest batch at the wrap point. Compare the instructions saved in the ISR against any extra padding.

## 3. Preserve arithmetic flags on the common IRQ path

Use `CPSE` to test equality of ring indices, and choose data movement instructions that preserve arithmetic flags. A handwritten ISR can skip saving SREG if every path to RETI leaves the relevant flags intact. Where the target has suitable GPIOR registers, an ISR with exclusive access to them and no nested interrupts can use them to save temporary state. Verify the empty and wrap paths and the interrupted program state. Count interrupt acceptance, vector dispatch, and RETI separately from time spent waiting for the interrupted instruction or for interrupts to be enabled.

## 4. Bound the longest gap between FIFO publications

Size the FIFO for the worst-case producer delay. Splitting a long refill into smaller batches can reduce the longest time the consumer waits for new data. Measure FIFO headroom at full load with IRQs enabled, varied input packets, and actual SPI and timer rates. Include prefill, startup, reset, and slow paths even when the average throughput meets the deadline.

## 5. Prevent timer period error from accumulating

When the sample rate does not divide the timer clock, accumulate the fractional remainder and vary the integer intervals accordingly. For example, a desired period of 12.5 timer ticks can alternate between 12 and 13. Measure both long-term drift and individual sample jitter, which also depends on interrupt latency. Check when the timer accepts a compare update and what happens when an update is late.

[Source files and official references](sources.md)
