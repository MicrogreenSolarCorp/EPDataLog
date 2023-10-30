# Project Build and Release Instructions

## Compilation Instructions

### For Mewyeah Windows:
```bash
cd ./Mewyeah/windows
gcc -std=c99 -o EPDataLogMewyeahWindows ../../common/readProgramParams.c ../../common/windows/connection.c ../../common/windows/outputToCsv.c ../common/getData.c main.c
```

### For Daly Windows:
```bash
cd ./Daly/windows
gcc -std=c99 -o EPDataLogDalyWindows ../../common/readProgramParams.c ../../common/windows/connection.c ../../common/windows/outputToCsv.c ../common/getData.c main.c
```

### For Mewyeah MacOS:
```bash
cd ./Mewyeah/macos
gcc -std=c99 -o EPDataLogMewyeahMacOS ../../common/readProgramParams.c ../../common/macos/connection.c ../../common/macos/outputToCsv.c ../common/getData.c main.c
```

### For Daly MacOS:
```bash
cd ./Daly/macos
gcc -std=c99 -o EPDataLogDalyMacOS ../../common/readProgramParams.c ../../common/macos/connection.c ../../common/macos/outputToCsv.c ../common/getData.c main.c
```

## Version Tagging and Pushing
```
git tag -a EPDataLog-v1.0.0 -m "Released v1.0.0 for all 4 program variants"
 push origin EPDataLog-v1.0.0 
```
