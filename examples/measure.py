"""Build paired AVR examples and report linked sizes without using a programmer."""

from pathlib import Path
import re
import subprocess


EXAMPLES = Path(__file__).resolve().parent
OUTPUT = EXAMPLES.parent / "build" / "measure"
PAIRS = (
    ("table-ram", "table-flash"),
    ("channels-runtime", "channels-mono"),
    ("compare-split", "compare-chain"),
)


def run(*args):
    return subprocess.run(
        args, cwd=OUTPUT, text=True, stdout=subprocess.PIPE, check=True
    ).stdout


def build(probe, variant, name):
    elf = f"{name}.elf"
    sources = [str(EXAMPLES / "probe.c")]
    if probe == 3:
        sources.append(str(EXAMPLES / "compare.S"))
    run(
        "avr-gcc", "-mmcu=atmega328p", "-Os", "-Wall", "-Wextra", "-Werror",
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
    # These examples use the stock ATmega328P linker script.
    data = sections.get(".data", 0)
    return sections[".text"] + data, data + sections.get(".bss", 0) + sections.get(".noinit", 0)


def main():
    OUTPUT.mkdir(parents=True, exist_ok=True)
    print(run("avr-gcc", "--version").splitlines()[0])
    print(f"{'Image':22s} {'Flash (B)':>10s} {'Static SRAM (B)':>16s}")
    for probe, names in enumerate(PAIRS, 1):
        for variant, name in enumerate(names):
            flash, sram = build(probe, variant, name)
            print(f"{name:22s} {flash:10d} {sram:16d}")


if __name__ == "__main__":
    main()
