# Project Build and Release Instructions

## Compilation Instructions

### Note: v1-1 => v1.1, the period is removed because the period leads to issues with the file extionsion

### For Mewyeah Windows:
```bash
cd ./Mewyeah/windows
gcc -std=c99 -o EPDataLogMewyeahWindows_vX-X ../../common/readProgramParams.c ../../common/windows/connection.c ../../common/windows/outputToCsv.c ../common/getData.c main.c
```

### For Mewyeah MacOS:
```bash
cd ./Mewyeah/macos
gcc -std=c99 -o EPDataLogMewyeahMacOS_vX-X ../../common/readProgramParams.c ../../common/macos/connection.c ../../common/macos/outputToCsv.c ../common/getData.c main.c
```

### For Daly Windows:
```bash
cd ./Daly/windows
gcc -std=c99 -o EPDataLogDalyWindows_vX-X ../../common/readProgramParams.c ../../common/windows/connection.c ../../common/windows/outputToCsv.c ../common/getData.c main.c
```

### For Daly MacOS:
```bash
cd ./Daly/macos
gcc -std=c99 -o EPDataLogDalyMacOS_vX-X ../../common/readProgramParams.c ../../common/macos/connection.c ../../common/macos/outputToCsv.c ../common/getData.c main.c
```

## Version Tagging and Pushing
```
git tag -a EPDataLog_vX.X -m "Released vX.X for all 4 program variants"
git push origin EPDataLog_vX.X
```

## Important Notes
- Whenever a new version of the executable is built and released, you must update the run.bat file so that the version number matches, or the run.bat file won't work.
- Whenever a zip file is created for a release, **Make sure the README.md file is bundled in there**. Otherwise, the user won't know how to use the program if they can't access the README on github.