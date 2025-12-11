# WSJT-SWISS Build Notes

## GitHub Actions Workflows

| Workflow | Trigger | Purpose |
|----------|---------|---------|
| **Build WSJT-X Windows Installer** | `v*` tags, manual | Production builds |
| **Build NSIS Test** | manual only | Test NSIS/installer changes |

## Release Process

1. Commit changes to `master`
2. Create and push tag:
   ```bash
   git tag v2.x.x
   git push origin v2.x.x
   ```
3. Download ZIP artifact from Actions

## Re-trigger Failed Build

```bash
git tag -d v2.x.x
git push origin :refs/tags/v2.x.x
git tag v2.x.x
git push origin v2.x.x
```

## Key Files

| File | Purpose |
|------|---------|
| `CMakeCPackOptions.cmake.in` | NSIS installer config (settings import, icons, etc.) |
| `.github/workflows/build-windows.yml` | Main build workflow |
| `.github/workflows/build-nsis-test.yml` | NSIS testing workflow |

## Settings Import Logic

On install, copies `C:\wsjtx\WSJT-X.ini` → `%LOCALAPPDATA%\WSJT-SWISS\WSJT-SWISS.ini`
**Only if** WSJT-SWISS.ini does NOT already exist.

## Build Output

- Installer: `wsjtx-{version}-win64.exe`
- Artifact: `wsjtx-swiss-{version}.zip` (contains the .exe)
