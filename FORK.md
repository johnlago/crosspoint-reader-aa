# Personal fork notes

This is a personal fork of CrossPoint Reader for one EEGO Reader A4. It adds an
Off / Standard / Soft choice for text anti-aliasing. Soft uses the gray method
from EEGO Beta 7. The fork is not meant to go upstream.

## Repositories and branches

| Repository | Remote | Branch | Contents |
| --- | --- | --- | --- |
| CrossPoint, `johnlago/crosspoint-reader-aa` | `origin` | `develop` | Exact mirror of upstream `develop`. No fork changes. |
| | | `personal` | `develop` plus the fork changes. Build and flash this branch. |
| Upstream CrossPoint, `crosspoint-reader/crosspoint-reader` | `upstream` | `develop` | Source of updates. |
| SDK, `johnlago/freeink-sdk` (submodule `freeink-sdk/`) | `origin` | `personal` | The SDK commit upstream CrossPoint pins, plus the Beta 7 gray change. |
| Upstream SDK, `Free-Ink/freeink-sdk` | `upstream` | `main` | Source of SDK updates. |

On `personal`, `.gitmodules` points `freeink-sdk` at `johnlago/freeink-sdk`.

## Fork changes

CrossPoint (`personal`):

- `src/CrossPointSettings.h`: `TEXT_ANTI_ALIASING` enum (Off, Standard, Soft).
- `src/ReaderTextAntiAliasing.h`: Soft is offered only on the EEGO A4.
- `src/SettingsList.cpp`, `src/activities/settings/TextSettingsActivity.*`:
  the setting row and its popup.
- `src/activities/reader/EpubReaderActivity.cpp`: Soft turns on overlay full
  tone (`setOverlayFullTone`).
- `lib/I18n/translations/english.yaml`, `USER_GUIDE.md`.

SDK (`personal`):

- `libs/display/FreeInkDisplay/src/driver/Uc8279cA4Driver.*`: full-tone pages
  use `writeGray3PageUp`, `loadGray3UpLut` and `loadGray3UpDarkLut` (grays
  built up from white, ported from SDK `77ae8c7`). Single-gray pages use the
  upstream code unchanged. Upstream's two-gray "lighten" path is left in
  place, unused, to keep merges small.
- `libs/display/FreeInkDisplay/src/lut/Uc8279cA4Luts.h`:
  `A4_UC8279C_GRAY3_UP_*` timings. Light 3, dark extra 2, settle 4 are
  Beta 7's values and the owner's choice.
- `libs/display/FreeInkDisplay/test/host/`: A4 host test updated for the
  new two-gray codes.

## Rules

- Standard must match upstream exactly. Do not change
  `A4_UC8279C_GRAY3_GRAY_WHITE_FRAMES` or the single-gray path.
- Soft keeps the Beta 7 method and timings unless the owner asks for a change.
- If an upstream change touches the A4 Gray3 code, the full-tone API, or the
  reader's full-tone scope, stop and explain it to the owner before choosing.

## Syncing with upstream

1. CrossPoint: `git fetch upstream`, fast-forward `develop` to
   `upstream/develop`, then merge `develop` into `personal`.
2. Note the SDK commit upstream now pins:
   `git ls-tree upstream/develop freeink-sdk`.
3. SDK: in `freeink-sdk/`, `git fetch upstream`, check out `personal`, merge
   that commit. Expect conflicts mostly in `Uc8279cA4Driver.cpp/.h`,
   `Uc8279cA4Luts.h` and `test_uc8279c_a4.cpp`.
4. Run the A4 host test:
   `python3 libs/display/FreeInkDisplay/test/host/run_uc8279c_a4.py`.
5. CrossPoint: stage `freeink-sdk` at the new SDK `personal` commit. Keep the
   fork URL in `.gitmodules` if it conflicts.
6. Build: `.venv/bin/pio run -e eego_a4` (see the toolchain note below).
7. Flash: `.venv/bin/pio run -e eego_a4 -t upload`. The owner checks Soft,
   Standard and image pages on the device.
8. Commit and push both `personal` branches, and `develop`, after the
   owner's approval.

Watch these files when merging: `Uc8279cA4Driver.cpp`, `Uc8279cA4Driver.h`,
`Uc8279cA4Luts.h`, `FreeInkDisplay` `setOverlayFullTone`,
`EpubReaderActivity.cpp` (`FullToneScope`), `TextSettingsActivity.cpp`,
`SettingsList.cpp`, `CrossPointSettings.h`.

## Toolchain

Use the project virtualenv `.venv` (pioarduino Core 6.1.19). The PlatformIO
on `PATH` (6.2.x) fails on the `default` target with
`No module named 'SCons.Tool.FortranCommon'`. If `~/.platformio/penv` is
recreated, pin it again:
`uv pip install --python ~/.platformio/penv/bin/python 'pioarduino==6.1.19'`.
