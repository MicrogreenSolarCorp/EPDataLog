# Project Build and Release Instructions

Note: v1-1 => v1.1, the period is removed because the period leads to issues with the file extionsion

## Compilation Instructions

The following commands will compile, and zip the executable + README into a zip file, located in the same directory as main.c.

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

### For Orion Windows:
```bash
make windows_orion VERSION=vX-X
```

### For Orion MacOS:
```bash
make mac_orion VERSION=vX-X
```

### To delete all executables and zip files:
```bash
make mac_clean
```
or
```bash
make windows_clean
```

## Cleanup Commands
### To delete all windows executables and thier zip folders:
```bash
make windows_clean
```

### To delete all macos executables and thier zip folders:
```bash
make mac_clean
```

### To delete all csv files that may have been created when testing/running the program (will not delete SampleLog.csv):
```bash
make clean_csv
```


## Version Tagging and Pushing
```
git tag -a EPDataLog_vX.X -m "Released vX.X for all 6 program variants"
git push origin EPDataLog_vX.X
```

## Important Notes
- For some reason, when the Mac execuatables are zipped on windows, they cannot be opened properly on Mac. Thus, make sure to zip the Mac executables on Mac.
- The MacOS versions of the software will work for both x86 and arm based Macs. If curious, feel free look in the make file to see how this is achieved.
- Orion BMS sends strings instead of byte arrays, but since they're both sent as unsigned char *, there shouldn't be any issues.
- Orion BMS requires EVEN parity when setting up the COM port connection with the BMS. This is done in the connection.c file.
- Alarm codes are not displayed with Orion BMS.