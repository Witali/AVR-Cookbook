# AVR-Cookbook

[Русская версия](README_ru.md)

Practical optimizations for classic 8-bit AVR microcontrollers. Each recipe explains when the technique applies, what it costs, and how to verify it on your target.

| When you need to… | Start with… |
| --- | --- |
| Find where memory and CPU time go | [Measurement guide](docs/measurement.md) for ELF files, link maps, listings, stack usage, and worst-case paths |
| Save SRAM or Flash, or reduce EEPROM wear | [Memory and configuration](docs/memory.md) for Flash tables, build-time specialization, and circular journals |
| Save a few instructions or cycles | [AVR assembly](docs/assembly.md) for flags, registers, pointers, branches, and the ABI |
| Reduce IRQ latency or prevent FIFO underruns | [Interrupts and streaming](docs/realtime.md) for timers, FIFO sizing, prefill, and latency bounds |
| Fit DSP into the cycle budget | [Integer arithmetic](docs/arithmetic.md) for bounded ranges, MAC, reused products, and offline computation |
| Fit a bootloader into its Flash region | [Bootloaders and protocols](docs/bootloaders.md) for shared code, placement, entry conditions, and race tests |

See the official AVR references for device and toolchain details. The DSP index groups techniques for arithmetic, filters, buffering, and playback. [AVR references](docs/sources.md) · [DSP index](docs/dsp-index.md)

To run the seven comparisons for ATmega328P and ATtiny85, install Python 3.10+, AVR-GCC, AVR-LibC, and AVR binutils, then run the measurement script.

```sh
python3 examples/measure.py
```

The script builds both variants of each example on the host and saves the ELF files, link maps, and disassembly in `build/measure/`. [What the examples compare](examples/README.md)

The example sizes and cycle counts apply to the documented builds. Use the measurement guide to evaluate the recipes on other targets. [Measurement guide](docs/measurement.md)

## Use the examples in another repository

From the root of your Git repository, start with a clean working tree and choose an unused path for the cookbook. Run the first command once to add the subtree, then run the second whenever you want to bring in updates.

```sh
git subtree add --prefix third_party/AVR-Cookbook https://github.com/Witali/AVR-Cookbook.git main --squash
git subtree pull --prefix third_party/AVR-Cookbook https://github.com/Witali/AVR-Cookbook.git main --squash
```

The examples are copied into your repository alongside the cookbook. To rerun the comparisons from your repository root, run `python3 third_party/AVR-Cookbook/examples/measure.py`. Keep the same prefix and `--squash` option when pulling updates. [Git subtree reference](https://github.com/git/git/blob/master/contrib/subtree/git-subtree.adoc)

To share an improvement, fork AVR-Cookbook, change the relevant example under `examples/`, and open a pull request. Describe the AVR target, the change, and how you checked it. Update both language versions if the documented behavior or measurements change.

## License

The documentation and examples are available under the MIT License. Keep the copyright and license notice when distributing copies or substantial portions. [LICENSE](LICENSE)
