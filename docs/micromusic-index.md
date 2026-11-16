# MicroMusic topic index

[Русская версия](micromusic-index_ru.md)

These specialized audio techniques depend on the format and the decoder's arithmetic precision. The names in the tables are files from the MicroMusic cookbook under `docs/avr-asm-cookbook/`, with the `.md` extension omitted. [Source revision](sources.md)

Run this example from the MicroMusic repository root.

```sh
git show 1fe7048d7b29:docs/avr-asm-cookbook/speex-bounded-divide.md
```

## UM88 and small players

| Goal | Source notes |
| --- | --- |
| Shorten the ISR and reduce memory accesses | `um88-page-fifo`, `um88-flag-neutral-irq`, `um88-platform` |
| Specialize hardware output and control code | `um88-configured-channels`, `um88-control-size` |
| Remove the gain table on AVR25 | `um88-table-free-gain` |
| Budget for small devices and set PWM levels | `tiny-speech-budget`, `pwm-output-level` |

## LPX

| Goal | Source notes |
| --- | --- |
| Choose a multiplication implementation and tune small kernels | `lpx-multiply-selection`, `lpx-avr25`, `lpx-small-kernel-boundaries`, `lpx-avr-size-screen`, `lpx-decode-simplification`, `lpx-fixed-c-realtime` |
| Fit a streaming player in RAM and Flash | `lpx-sram512`, `lpx-tiny88-budget`, `lpx-producer-budget`, `lpx-universal-tiny88`, `lpx-release-player` |
| Read the stream and choose data placement | `lpx-program-flash`, `lpx-external-spi` |
| Reduce work on LSFs, phase, and pitch period | `lpx-lsf-compact`, `lpx-v5-grid-cosine`, `lpx-polynomial-units`, `lpx-pitch-history`, `lpx-period-excitation` |
| Handle gain, short streams, and truncation | `lpx-host-gain`, `lpx-scale-repeat`, `lpx-compact-error-guard` |
| Compare formats and quality under a bitrate limit | `lpx-speech-compact`, `lpx-speech-v4`, `lpx-atmega328p-v9`, `lpx-v9`, `lpx-lowrate-quality`, `lpx-dialogue` |

## Speex

| Goal | Source notes |
| --- | --- |
| Reduce overhead in polynomial evaluation, filters, and MAC | `speex-angle`, `speex-lsp-twin`, `speex-iir-seed`, `speex-iir-groups`, `speex-finite-lpc-response` |
| Split QMF computations or reuse their results | `speex-qmf-input`, `speex-qmf-common-product`, `speex-qmf-split-accumulators`, `speex-hb-interpolation` |
| Exploit narrow ranges and finite alphabets | `speex-bounded-divide`, `speex-lpc-ratio-divide`, `speex-finite-alphabet-fold`, `speex-finite-gain`, `speex-finite-hb-ratio`, `speex-finite-pitch-root`, `speex-bounded-forced-math` |
| Specialize prediction for the mode or state | `speex-zero-prediction`, `speex-one-tap`, `speex-prepared-dispatch`, `speex-forced-history` |
| Simplify excitation, noise generation, and division for the mean | `speex-five-symbol-noise`, `speex-power-two-noise`, `speex-noise-history`, `speex-lcg-products`, `speex-complementary-mean` |
| Publish processed data more often and read it over SPI | `speex-lowrate-fusion`, `speex-quarter-refills`, `speex-spi-streaming` |

The notes cover both isolated prototypes and full decoder results. Before applying a technique, check the profile, the source of the measurements, and the limits given in the note.
