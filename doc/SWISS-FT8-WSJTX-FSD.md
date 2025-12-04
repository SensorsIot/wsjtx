# WSJT-SWISS - Functional Specification Document

## Feature Summary

| Feature | Description |
|---------|-------------|
| Swiss FT8 Contest Mode | Exchange Swiss canton codes using FT8 protocol extension (i3=0, n3=7) |
| 26 Canton Support | All Swiss cantons supported with 5-bit encoding |
| Auto-Population of TX Messages | TX message fields automatically populated with canton code when Swiss FT8 Contest mode is active |
| ADIF Logging | Canton exchange logged with MY_CANTON and HIS_CANTON fields |

---

## 1. Swiss FT8 Contest Mode

### 1.1 Purpose
WSJT-SWISS is a fork of WSJT-X designed for the Swiss FT8 Contest. It enables amateur radio operators to exchange Swiss canton codes as part of the FT8 digital mode protocol.

### 1.2 Mode Selection
- Location: Settings → Advanced → Special Operating Activity
- Radio button: "Swiss FT8 Contest"
- Canton dropdown: All 26 cantons in alphabetical order
- Default selection: BL (Basel-Landschaft)

### 1.3 FT8 Protocol Extension
Swiss contest messages use **i3=0, n3=7** (a previously reserved slot in the FT8 protocol).

Message format:
```
0.7   HB9BLA HB9XYZ ZH                   28 28 15              71   Swiss FT8 Contest
```

### 1.4 Bit Structure (77 bits total)
| Bits | Size | Content |
|------|------|---------|
| 0-27 | 28 bits | Callsign 1 |
| 28-55 | 28 bits | Callsign 2 |
| 56-60 | 5 bits | Canton code (0-25) |
| 61-70 | 10 bits | Reserved |
| 71-73 | 3 bits | n3 field (value: 7) |
| 74-76 | 3 bits | i3 field (value: 0) |

### 1.5 QSO Message Sequence

A typical Swiss FT8 Contest QSO follows this sequence:

| Step | Station A (ZH) | Station B (BE) |
|------|----------------|----------------|
| 1 | `CQ ZH HB9AAA JN47` | |
| 2 | | `HB9AAA HB9BBB -07` |
| 3 | `HB9BBB HB9AAA -06` | |
| 4 | | `HB9AAA HB9BBB BE` |
| 5 | `HB9BBB HB9AAA ZH` | |
| 6 | | `HB9AAA HB9BBB RR73` |

Steps 4 and 5 use the structured i3=0, n3=7 encoding for canton exchange.

---

## 2. 26 Canton Support

### 2.1 Canton Codes
The system supports all 26 Swiss cantons, encoded as values 0-25:

| Code | Canton | Code | Canton | Code | Canton |
|------|--------|------|--------|------|--------|
| 0 | AG (Aargau) | 9 | GR (Graubünden) | 18 | SZ (Schwyz) |
| 1 | AI (Appenzell I.Rh.) | 10 | JU (Jura) | 19 | TG (Thurgau) |
| 2 | AR (Appenzell A.Rh.) | 11 | LU (Luzern) | 20 | TI (Ticino) |
| 3 | BE (Bern) | 12 | NE (Neuchâtel) | 21 | UR (Uri) |
| 4 | BL (Basel-Landschaft) | 13 | NW (Nidwalden) | 22 | VD (Vaud) |
| 5 | BS (Basel-Stadt) | 14 | OW (Obwalden) | 23 | VS (Valais) |
| 6 | FR (Fribourg) | 15 | SG (St. Gallen) | 24 | ZG (Zug) |
| 7 | GE (Genève) | 16 | SH (Schaffhausen) | 25 | ZH (Zürich) |
| 8 | GL (Glarus) | 17 | SO (Solothurn) | | |

### 2.2 Default Canton
The default canton is **BL** (Basel-Landschaft), pre-selected for new installations.

### 2.3 Configuration Storage
| Setting | Description | Default |
|---------|-------------|---------|
| `Swiss_My_Canton` | Operator's canton code | BL |

---

## 3. Auto-Population of TX Messages

### 3.1 Functionality
When Swiss FT8 Contest mode is active, the TX message fields are automatically populated with the operator's canton code instead of the grid locator.

### 3.2 Trigger Events
Auto-population occurs when:
- Double-clicking a station in the decode list
- Using the "Generate Std Msgs" function
- Responding to a CQ call

### 3.3 Message Transformation
The canton code from Settings replaces the grid square in contest exchange messages:

| Mode | Message Format |
|------|----------------|
| Standard FT8 | `HB9BBB HB9AAA JN47` |
| Swiss Contest | `HB9BBB HB9AAA ZH` |

---

## 4. ADIF Logging

### 4.1 Additional ADIF Fields
Swiss contest QSOs include additional ADIF fields for canton exchange:

| Field | Length | Description | Example |
|-------|--------|-------------|---------|
| `MY_CANTON` | 2 | Operator's canton | `ZH` |
| `HIS_CANTON` | 2 | Contacted station's canton | `BE` |

### 4.2 ADIF Record Example
```
<CALL:6>HB9BBB <MODE:3>FT8 <RST_SENT:3>-06 <RST_RCVD:3>-07 <MY_CANTON:2>ZH <HIS_CANTON:2>BE
```

---

## 5. User Interface

### 5.1 Application Identity
- Window title: "WSJT-SWISS"
- Distinguishes this fork from standard WSJT-X

### 5.2 Progress Bar TX/RX Indicator
The timing progress bar changes color based on operating state:
- **Green**: Receiving (RX)
- **Red**: Transmitting (TX)

This provides immediate visual feedback of the current transmit state.

---

## 6. Source Code Changes

### 6.1 Modified Files

| File | Changes |
|------|---------|
| `main.cpp` | Application name set to "WSJT-SWISS" |
| `Configuration.hpp` | Added `SWISS_FT8_CONTEST` to `SpecialOperatingActivity` enum; added `swiss_my_canton()` accessor |
| `Configuration.cpp` | Canton storage, UI handlers, default value (BL) |
| `Configuration.ui` | Swiss FT8 Contest radio button and canton dropdown in Special Operating Activity |
| `widgets/mainwindow.cpp` | Canton substitution in `genStdMsgs()`; progress bar color change in `guiUpdate()` |
| `lib/77bit/packjt77.f90` | `pack77_07` and `unpack77` subroutines for i3=0, n3=7 message encoding/decoding |
| `logbook/logbook.cpp` | MY_CANTON and HIS_CANTON ADIF field generation |

### 6.2 Fortran Changes (packjt77.f90)

**New subroutine `pack77_07`:**
- Encodes Swiss contest messages (CALL1 CALL2 CANTON)
- Validates canton code (0-25)
- Packs into 77-bit structure with i3=0, n3=7

**Modified `unpack77`:**
- Decodes i3=0, n3=7 messages
- Extracts canton code from 5-bit field
- Reconstructs message string with canton abbreviation

### 6.3 Qt UI Changes (Configuration.ui)

**Special Operating Activity section:**
- New radio button: `rbSwiss_FT8_Contest` with text "Swiss FT8 Contest"
- New stacked widget page (index 10) for Swiss contest options
- Canton dropdown: `cbSwiss_My_Canton` (QComboBox) with all 26 cantons

**Widget properties:**
| Widget | Type | Properties |
|--------|------|------------|
| `rbSwiss_FT8_Contest` | QRadioButton | Text: "Swiss FT8 Contest", part of Special Op button group |
| `cbSwiss_My_Canton` | QComboBox | 26 items (AG-ZH), alphabetically sorted |

### 6.4 C++ Changes

**Configuration (Configuration.hpp/cpp):**
- New enum value: `SpecialOperatingActivity::SWISS_FT8_CONTEST`
- Canton stored in `swiss_my_canton_` member variable
- Settings key: `Swiss_My_Canton`
- Slot: `on_rbSwiss_FT8_Contest_clicked()` - switches to Swiss contest page
- Slot: `on_cbSwiss_My_Canton_currentIndexChanged()` - updates canton selection

**Main Window (mainwindow.cpp):**
- `genStdMsgs()`: Substitutes canton for grid when Swiss mode active
- `guiUpdate()`: Sets progress bar stylesheet based on `m_transmitting` state

---

## 7. Build Process

### 7.1 Dependencies
| Dependency | Version | Purpose |
|------------|---------|---------|
| Hamlib | 4.6.5 | Radio control, FlexRadio slice support |
| Qt | 5.x | GUI framework |
| FFTW3 | 3.x | FFT processing with OpenMP threading |
| MinGW64 | Latest | Windows compilation toolchain |
| NSIS | Latest | Windows installer creation |

### 7.2 Build Environment
- Platform: Windows (MSYS2/MinGW64)
- CMake generator: MinGW Makefiles
- Build type: Release
- Parallel jobs: 4

### 7.3 Build Steps
1. Configure with CMake
2. Build with `cmake --build . --config Release --parallel 4`
3. Install to staging directory
4. Bundle DLL dependencies with `windeployqt` and `ldd`
5. Restructure for flat layout (see 7.5)
6. Create NSIS installer with `cpack -G NSIS`
7. Create portable ZIP

### 7.4 Portable Package Structure

The portable distribution uses a clean directory layout:

```
wsjtx-swiss/
├── wsjtx-swiss.cmd          <-- launcher script (user clicks this)
├── message_aggregator.cmd   <-- launcher for helper app
├── lib/                     <-- all binaries
│   ├── wsjtx-swiss.exe
│   ├── message_aggregator.exe
│   ├── *.dll
│   ├── qt.conf
│   ├── platforms/
│   ├── imageformats/
│   ├── styles/
│   ├── audio/
│   └── mediaservice/
└── share/                   <-- data files
    └── wsjtx/
```

**Design rationale:**
- User sees only launcher scripts and folders at top level
- All DLLs hidden in `lib/` subfolder for clean appearance
- Launcher scripts set working directory to `lib/` before executing
- `qt.conf` in `lib/` tells Qt where to find plugins

### 7.5 CI/CD Pipeline (GitHub Actions)

**Workflow Files:**
| File | Purpose |
|------|---------|
| `.github/workflows/build-windows.yml` | Main WSJT-SWISS build workflow |
| `.github/workflows/build-hamlib.yml` | Hamlib 4.6.5 build workflow |

#### build-windows.yml
Builds WSJT-SWISS application.

**Trigger Events:**
- Push to Swiss branch
- Pull requests to Swiss branch
- Manual workflow dispatch

**Artifacts Produced:**
| Artifact | Description |
|----------|-------------|
| `wsjtx-swiss-installer` | NSIS Windows installer (.exe) |
| `wsjtx-swiss-portable` | Portable ZIP distribution |

**Retention:** 30 days

#### build-hamlib.yml
Builds Hamlib 4.6.5 from source for MinGW64.

**Purpose:** Provides prebuilt Hamlib binaries with FlexRadio slice support.

**Artifacts Produced:**
| Artifact | Description |
|----------|-------------|
| `hamlib-mingw64-4.6.5` | Prebuilt Hamlib libraries and DLLs |

The main build workflow downloads these prebuilt Hamlib binaries from a GitHub release.

---

## 8. Compatibility

| Scenario | Compatibility |
|----------|---------------|
| WSJT-SWISS ↔ WSJT-SWISS | Full Swiss contest support |
| WSJT-SWISS ↔ WSJT-X | Standard FT8 modes only; Swiss contest messages (i3=0, n3=7) not decoded by WSJT-X |
| Standard FT8/FT4/etc. | Fully compatible with all WSJT-X versions |
