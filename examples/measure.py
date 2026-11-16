"""Build paired AVR examples and report linked sizes without using a programmer."""

from pathlib import Path
import re
import subprocess


EXAMPLES = Path(__file__).resolve().parent
OUTPUT = EXAMPLES.parent / "build" / "measure"
PAIRS = (
    ("atmega328p", ("probe.c",), ("table-ram", "table-flash")),
    ("atmega328p", ("probe.c",), ("channels-runtime", "channels-mono")),
    ("atmega328p", ("probe.c", "compare.S"), ("compare-split", "compare-chain")),
    ("atmega328p", ("blend.c",), ("blend-two-products", "blend-one-product")),
    ("attiny85", ("stride.c",), ("stride-multiply", "stride-table")),
    ("atmega328p", ("fifo-block.c",), ("fifo-wrap-byte", "fifo-wrap-block")),
    ("atmega328p", ("select-probe.c", "select-constant.S"), ("select-branch", "select-skip")),
)


def run(*args):
    return subprocess.run(
        args, cwd=OUTPUT, text=True, stdout=subprocess.PIPE, check=True
    ).stdout


def build(mcu, filenames, probe, variant, name):
    elf = f"{name}.elf"
    sources = [str(EXAMPLES / filename) for filename in filenames]
    run(
        "avr-gcc", f"-mmcu={mcu}", "-Os", "-Wall", "-Wextra", "-Werror",
        "-ffunction-sections", "-fdata-sections",
        f"-DPROBE={probe}", f"-DVARIANT={variant}",
        f"-Wl,--gc-sections,-Map={name}.map", "-o", elf, *sources,
    )
    listing = run("avr-objdump", "-h", "-d", elf)
    (OUTPUT / f"{name}.lst").write_text(listing, encoding="utf-8")
    sections = {
        name: int(size)
        for name, size in re.findall(r"^(\.[\w.]+)\s+(\d+)\s+", run("avr-size", "-A", elf), re.M)
    }
    # These examples use the stock classic-AVR linker scripts.
    data = sections.get(".data", 0)
    return sections[".text"] + data, data + sections.get(".bss", 0) + sections.get(".noinit", 0)


def main():
    OUTPUT.mkdir(parents=True, exist_ok=True)
    print(run("avr-gcc", "--version").splitlines()[0])
    print(f"{'MCU':12s} {'Image':22s} {'Flash (B)':>10s} {'Static SRAM (B)':>16s}")
    for probe, (mcu, filenames, names) in enumerate(PAIRS, 1):
        for variant, name in enumerate(names):
            flash, sram = build(mcu, filenames, probe, variant, name)
            print(f"{mcu:12s} {name:22s} {flash:10d} {sram:16d}")


if __name__ == "__main__":
    main()
