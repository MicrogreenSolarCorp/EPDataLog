# Project Build and Release Instructions

## Compilation Instructions

Note: v1-1 => v1.1, the period is removed because the period leads to issues with the file extionsion

### For Mewyeah Windows:
```bash
make windows_mewyeah VERSION=vX-X
```

### For Mewyeah MacOS:
```bash
make mac_mewyeah VERSION=vX-X
```

### For Daly Windows:
```bash
make windows_daly VERSION=vX-X
```

### For Daly MacOS:
```bash
make mac_daly VERSION=vX-X
```

## Version Tagging and Pushing
```
git tag -a EPDataLog_vX.X -m "Released vX.X for all 4 program variants"
git push origin EPDataLog_vX.X
```

## Important Notes
- Whenever a zip file is created for a release, **Make sure the README.md file is bundled in there**. Otherwise, the user won't know how to use the program if they can't access the README on github.
- For some reason, when the Mac execuatables are zipped on windows, they cannot be opened properly on Mac. Thus, make sure to zip the Mac executables on Mac.
- The MacOS versions of the software will work for both x86 and arm based Macs. If you are curious, you may look in the make file to see how this is achieved.
