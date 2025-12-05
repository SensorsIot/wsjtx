# WSJT-SWISS

[![Build Status](https://github.com/SensorsIot/wsjtx/actions/workflows/build-windows.yml/badge.svg?branch=Swiss)](https://github.com/SensorsIot/wsjtx/actions/workflows/build-windows.yml)
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![Platform](https://img.shields.io/badge/Platform-Windows-0078D6?logo=windows)](https://github.com/SensorsIot/wsjtx/releases)
[![WSJT-X](https://img.shields.io/badge/Based%20on-WSJT--X%202.7.0-green)](https://wsjt.sourceforge.io/wsjtx.html)

A fork of [WSJT-X](https://wsjt.sourceforge.io/wsjtx.html) with **Swiss FT8 Contest** support for exchanging canton codes.

---

## Features

| Feature | Description |
|---------|-------------|
| **Swiss FT8 Contest Mode** | Exchange Swiss canton codes using FT8 protocol extension (i3=0, n3=7) |
| **26 Canton Support** | All Swiss cantons supported with 5-bit encoding |
| **Auto TX Messages** | TX message fields automatically populated with canton code |
| **ADIF Logging** | Canton exchange logged with `MY_CANTON` and `HIS_CANTON` fields |
| **TX/RX Indicator** | Progress bar turns red during transmit, green during receive |

---

## Download

Download the latest release from the [GitHub Actions artifacts](https://github.com/SensorsIot/wsjtx/actions/workflows/build-windows.yml):

| Package | Description |
|---------|-------------|
| `wsjtx-swiss-installer` | Windows installer (.exe) |
| `wsjtx-swiss-portable` | Portable ZIP - no installation required |

---

## Swiss FT8 Contest Mode

### Enabling the Mode

1. Go to **Settings** > **Advanced** > **Special Operating Activity**
2. Select **Swiss FT8 Contest**
3. Choose your canton from the dropdown (default: BL)

### QSO Sequence

A typical Swiss FT8 Contest QSO:

```
Station A (ZH)              Station B (BE)
─────────────────────────────────────────────
CQ ZH HB9AAA JN47
                            HB9AAA HB9BBB -07
HB9BBB HB9AAA -06
                            HB9AAA HB9BBB BE    ← Canton exchange
HB9BBB HB9AAA ZH            ← Canton exchange
                            HB9AAA HB9BBB RR73
```

### Supported Cantons

| Code | Canton | Code | Canton | Code | Canton |
|------|--------|------|--------|------|--------|
| AG | Aargau | GR | Graubünden | SZ | Schwyz |
| AI | Appenzell I.Rh. | JU | Jura | TG | Thurgau |
| AR | Appenzell A.Rh. | LU | Luzern | TI | Ticino |
| BE | Bern | NE | Neuchâtel | UR | Uri |
| BL | Basel-Landschaft | NW | Nidwalden | VD | Vaud |
| BS | Basel-Stadt | OW | Obwalden | VS | Valais |
| FR | Fribourg | SG | St. Gallen | ZG | Zug |
| GE | Genève | SH | Schaffhausen | ZH | Zürich |
| GL | Glarus | SO | Solothurn | | |

---

## Compatibility

| Scenario | Status |
|----------|--------|
| WSJT-SWISS ↔ WSJT-SWISS | Full Swiss contest support |
| WSJT-SWISS ↔ WSJT-X | Standard FT8 modes only |
| Standard FT8/FT4/etc. | Fully compatible |

> **Note:** Swiss contest messages (i3=0, n3=7) are not decoded by standard WSJT-X.

---

## Building from Source

### Requirements

- MSYS2 / MinGW64
- Qt 5.x
- Hamlib 4.6.5
- FFTW3
- Boost
- CMake

### Build Steps

```bash
mkdir build && cd build
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --parallel 4
```

---

## Documentation

See [SWISS-FT8-WSJTX-FSD.md](doc/SWISS-FT8-WSJTX-FSD.md) for the full Functional Specification Document.

---

## Credits

- **WSJT-X** by Joe Taylor, K1JT and the WSJT Development Team
- **Swiss FT8 Contest Mode** fork maintained by [SensorsIot](https://github.com/SensorsIot)

---

## License

This project is licensed under the GNU General Public License v3.0 - see the [COPYING](COPYING) file for details.

Based on WSJT-X, Copyright (C) 2001-2023 by Joe Taylor, K1JT.
