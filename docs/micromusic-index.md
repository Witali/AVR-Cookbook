# Предметный указатель MicroMusic

Здесь собраны более узкие аудиоприёмы, которые трудно перенести без знания формата и точности декодера. Имена в таблицах — файлы исходного справочника MicroMusic под `docs/avr-asm-cookbook/` без расширения `.md`, на [зафиксированной ревизии](sources.md).

Например, из корня MicroMusic:

```sh
git show 1fe7048d7b29:docs/avr-asm-cookbook/speex-bounded-divide.md
```

## UM88 и малые плееры

| Что требуется | Исходные заметки |
| --- | --- |
| Сократить IRQ и обращения к памяти | `um88-page-fifo`, `um88-flag-neutral-irq`, `um88-platform` |
| Специализировать аппаратный вывод и управление | `um88-configured-channels`, `um88-control-size` |
| Убрать таблицу усиления в AVR25 | `um88-table-free-gain` |
| Подобрать бюджет малой платформы и уровни PWM | `tiny-speech-budget`, `pwm-output-level` |

## LPX

| Что требуется | Исходные заметки |
| --- | --- |
| Выбрать реализацию умножения, оптимизировать малые ядра | `lpx-multiply-selection`, `lpx-avr25`, `lpx-small-kernel-boundaries`, `lpx-avr-size-screen`, `lpx-decode-simplification`, `lpx-fixed-c-realtime` |
| Уместить потоковый плеер в RAM и Flash | `lpx-sram512`, `lpx-tiny88-budget`, `lpx-producer-budget`, `lpx-universal-tiny88`, `lpx-release-player` |
| Читать поток и выбрать размещение данных | `lpx-program-flash`, `lpx-external-spi` |
| Уменьшить число операций с LSF, фазой и периодом | `lpx-lsf-compact`, `lpx-v5-grid-cosine`, `lpx-polynomial-units`, `lpx-pitch-history`, `lpx-period-excitation` |
| Обработать усиление, короткий поток и обрыв | `lpx-host-gain`, `lpx-scale-repeat`, `lpx-compact-error-guard` |
| Сопоставить форматы и качество при ограниченном битрейте | `lpx-speech-compact`, `lpx-speech-v4`, `lpx-atmega328p-v9`, `lpx-v9`, `lpx-lowrate-quality`, `lpx-dialogue` |

## Speex

| Что требуется | Исходные заметки |
| --- | --- |
| Снизить накладные расходы полиномов, фильтрации и MAC | `speex-angle`, `speex-lsp-twin`, `speex-iir-seed`, `speex-iir-groups`, `speex-finite-lpc-response` |
| Разделить вычисления QMF или использовать общий результат | `speex-qmf-input`, `speex-qmf-common-product`, `speex-qmf-split-accumulators`, `speex-hb-interpolation` |
| Использовать узкие диапазоны и конечные алфавиты | `speex-bounded-divide`, `speex-lpc-ratio-divide`, `speex-finite-alphabet-fold`, `speex-finite-gain`, `speex-finite-hb-ratio`, `speex-finite-pitch-root`, `speex-bounded-forced-math` |
| Специализировать предиктор по режиму или состоянию | `speex-zero-prediction`, `speex-one-tap`, `speex-prepared-dispatch`, `speex-forced-history` |
| Упростить возбуждение, шум и деление среднего | `speex-five-symbol-noise`, `speex-power-two-noise`, `speex-noise-history`, `speex-lcg-products`, `speex-complementary-mean` |
| Публиковать обработанные данные чаще, читать их по SPI | `speex-lowrate-fusion`, `speex-quarter-refills`, `speex-spi-streaming` |

Некоторые заметки описывают только изолированный прототип, а некоторые — результаты полного декодера. При переносе сверяйте профиль, источник замера и ограничения в самой заметке.
