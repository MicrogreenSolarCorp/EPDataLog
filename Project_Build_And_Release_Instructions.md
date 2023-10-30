# Project Build and Release Instructions

## Compilation Instructions

### For Mewyeah Windows:
```bash
cd ./Mewyeah/windows
gcc -std=c99 -o EPDataLogMewyeahWindows-v1.1 ../../common/readProgramParams.c ../../common/windows/connection.c ../../common/windows/outputToCsv.c ../common/getData.c main.c
```

### For Mewyeah MacOS:
```bash
cd ./Mewyeah/macos
gcc -std=c99 -o EPDataLogMewyeahMacOS-v1.1 ../../common/readProgramParams.c ../../common/macos/connection.c ../../common/macos/outputToCsv.c ../common/getData.c main.c
```

### For Daly Windows:
```bash
cd ./Daly/windows
gcc -std=c99 -o EPDataLogDalyWindows-v1.1 ../../common/readProgramParams.c ../../common/windows/connection.c ../../common/windows/outputToCsv.c ../common/getData.c main.c
```

### For Daly MacOS:
```bash
cd ./Daly/macos
gcc -std=c99 -o EPDataLogDalyMacOS-v1.1 ../../common/readProgramParams.c ../../common/macos/connection.c ../../common/macos/outputToCsv.c ../common/getData.c main.c
```

## Version Tagging and Pushing
```
git tag -a EPDataLog-vX.X -m "Released vX.X for all 4 program variants"
 push origin EPDataLog-vX.X
```
