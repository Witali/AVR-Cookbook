# Источники и границы проверки

Пути в таблицах относительны к исходным проектам. Их можно открыть через `git show <ревизия>:<путь>` в соответствующем клоне. Здесь приведены собственные объяснения и короткие учебные примеры; прошивки и проектные исходники не включены.

| Проект | Ревизия | Устройства в этой книге |
| --- | --- | --- |
| MicroMusic | `1fe7048d7b29` | ATtiny13A, ATtiny88, ATmega328P |
| optiboot | `e3f93466abd0` | ATmega328P/PB, ATtiny84/85, ATtiny841/1634 |
| MicroNucleus, вложенный репозиторий `src/micronucleus` | `b2ae7c84c490` | ATtiny88 |
| LPC-sound-codec | `360af1460785` | ATmega328P / Arduino Nano |

## MicroMusic

| Что искать | Файл | Свидетельство |
| --- | --- | --- |
| Обзор ASM, цепочка MUL и ABI | `docs/avr-asm-cookbook/README.md` | Размер, такты, сравнение PCM, IRQ в модели |
| FIFO в одной странице | `docs/avr-asm-cookbook/um88-page-fifo.md` | Изолированные размеры и модель IRQ |
| Флаги и GPIOR | `docs/avr-asm-cookbook/um88-flag-neutral-irq.md` | Такты, прерывания и крайние случаи в модели |
| Режимы mono и stereo | `docs/avr-asm-cookbook/um88-configured-channels.md` | Сборка и модель; C stereo пропускает дедлайн |
| Реализация UM88 | `ATtiny88/Music/universal/platform.c`, `ATtiny88/Music/universal/irq-avr.S`, `ATtiny88/Music/universal/storage.h` | Исходники платформы, ISR и интерфейса памяти |
| Выбор арифметического ядра | `Codecs/LPX/01_ACTIVE_Q0_CODEC/src/Lpx4Math.h`, `docs/avr-asm-cookbook/lpx-multiply-selection.md` | Дизассемблирование для AVR5 и AVR25 |
| Более узкие рецепты DSP | [Предметный указатель](micromusic-index.md) | Каждый исходный файл описывает свой профиль и тесты |

## optiboot

| Что искать | Файл | Свидетельство |
| --- | --- | --- |
| Политика сборки C и условные возможности | `optiboot/bootloaders/optiboot/Makefile`, `optiboot/bootloaders/optiboot/optiboot.c` | Исходники |
| 368/358/356/350 Б, длина и общий хвост | `docs/atmega328p/history/03_358_350_size_optimization/README.md` | Сборка и сравнение в модели инструкций |
| Актуальная сборка Mini/Middle/Full и код | `docs/mini/README.md`, `optiboot/bootloaders/optiboot/optiboot_mini.S` | Контракт профилей и реализация |
| Компоновка Virtual Boot, границы и матрица сборок | `docs/mini/VIRTUAL_BOOT.md`, `docs/mini/INTEGRATION_2026-11-10.md` | Сборки GCC и LLVM, модель команд и памяти |
| Middle и Full на реальном Nano | `docs/mini/HARDWARE_MIDDLE_ATMEGA328PB_2026-11-04.md`, `docs/mini/HARDWARE_FULL_ATMEGA328PB_2026-11-04.md` | Аппаратная проверка точных образов на ATmega328PB |

## MicroNucleus

Добавьте префикс `experimental/attiny88/` к путям ниже.

| Что искать | Файл | Свидетельство |
| --- | --- | --- |
| История ATtiny88 и различия веток | `README.md` | Сводка образов и уровней проверки |
| Склейка CRC/копирования, битовый I/O, STD, выравнивание | `variants/strict_usb_optimized/README.md`, `variants/strict_usb_optimized/micronucleus_t88_optimized.S` | Сборки и модель для 1262 Б без LED |
| USB, Flash и EEPROM на плате | `variants/strict_usb_optimized/HARDWARE_TEST.md` | Проверка предыдущего образа 1274 Б |
| Состояние SETUP, цепочки CPC, опросы и отвергнутые сокращения | `flash_only/docs/SIZE_REDUCTIONS.md`, `flash_only/source/micronucleus_t88_compact.S` | Отдельная ветка, модель, открытая квалификация USB |
| Проблема 1024-байтного прототипа | `flash_only/README.md` | Преждевременный SPM в модели |
| 512/32/16/0 Б для CRC16 | `block64/prototypes/streaming_crc16/CRC_TABLE_TRADEOFFS.md` | Изолированные ядра и бюджет приёма |

## LPC-sound-codec

| Проект | Файл | Что искать |
| --- | --- | --- |
| LPC-sound-codec | `arduino/Lpc2AvrDecoder/src/Lpc2AvrDecoder.cpp` | Таблицы Flash, подготовка кадров, таймеры и атомарный снимок счётчика |
| LPC-sound-codec | `arduino/Lpc2AvrDecoder/examples/LPC2_PROGMEM_Player/LPC2_PROGMEM_Player.ino` | Чтение потока по байту из программной памяти |

## Документация AVR

- Microchip: [ATmega328P, регистры и таблица BOOTSZ](https://ww1.microchip.com/downloads/en/DeviceDoc/ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061A.pdf), [ATtiny88, таймеры и I/O](https://ww1.microchip.com/downloads/en/DeviceDoc/doc8008.pdf), [сводка инструкций AVR](https://onlinedocs.microchip.com/oxy/GUID-06C36746-0136-4691-8940-69BFD41FCC56-en-US-1/GUID-64F3D141-2594-4755-9103-550108045801.html).
- GCC: [опции AVR и макросы целевого устройства в GCC 7.3](https://gcc.gnu.org/onlinedocs/gcc-7.3.0/gcc/AVR-Options.html).
- AVR-LibC: [программная память](https://avrdudes.github.io/avr-libc/avr-libc-user-manual/group__avr__pgmspace.html), [секции памяти](https://avrdudes.github.io/avr-libc/avr-libc-user-manual/mem_sections.html), [ABI и регистры](https://avrdudes.github.io/avr-libc/avr-libc-user-manual/FAQ.html#faq_reg_usage), [атомарные блоки](https://avrdudes.github.io/avr-libc/avr-libc-user-manual/group__util__atomic.html).
