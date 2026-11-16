# DSP technique index

[Русская версия](dsp-index_ru.md)

| Task | Technique | What to verify |
| --- | --- | --- |
| Scale a signal or waveform | Prove the range of each input and product. Compare lookup tables, additions, and hardware multiplication where available. [Integer arithmetic](arithmetic.md) | Rounding of negative values, saturation, size, and worst-case cycles |
| Run a filter or mixer | Start a MAC with its first product, reuse related products, and keep accumulators in registers. [Arithmetic](arithmetic.md) · [Assembly](assembly.md) | State and outputs at extrema, including order of rounding and clipping |
| Select a playback mode | Build fixed channel counts and store constant tables in Flash. [Memory and configuration](memory.md) | Unused register writes in the listing and the actual memory cost |
| Stream samples from storage | Move reads and frame preparation out of the ISR. Publish complete batches to a FIFO and size it for the slowest refill. [Interrupts and streaming](realtime.md) | FIFO headroom during startup and long gaps with real SPI and timer rates |
| Set the sample rate | Vary timer intervals using the accumulated fractional remainder. [Interrupts and streaming](realtime.md) | Instantaneous jitter and cumulative drift |
| Encode data ahead of time | Choose gain or filter parameters on the host and pass them in existing stream fields. [Integer arithmetic](arithmetic.md) | Bitrate, decoded output, and whether decode timing still meets the deadline |
