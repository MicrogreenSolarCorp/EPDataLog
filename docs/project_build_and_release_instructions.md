# Project Build and Release Instructions

## Compilation Instructions

Note: v1-1 => v1.1, the period is removed because the period leads to issues with the file extionsion

### For Mewyeah Windows:
```bash
cd ./Mewyeah/windows
gcc -std=c99 -o EPDataLog_Mewyeah_Windows_vX-X ../../common/getInput.c ../../common/windows/connection.c ../../common/windows/outputToCsv.c ../common/getData.c main.c
```

### For Mewyeah MacOS:
```bash
cd ./Mewyeah/macos

gcc -std=c99 -arch x86_64 -o EPDataLog_Mewyeah_MacOS_vX-X_x86 ../../common/getInput.c ../../common/macos/connection.c ../../common/macos/outputToCsv.c ../common/getData.c main.c

gcc -std=c99 -arch arm64 -o EPDataLog_Mewyeah_MacOS_vX-X_arm ../../common/getInput.c ../../common/macos/connection.c ../../common/macos/outputToCsv.c ../common/getData.c main.c

lipo -create -output EPDataLog_Mewyeah_MacOS_vX-X_universal EPDataLog_Mewyeah_MacOS_vX-X_x86 EPDataLog_Mewyeah_MacOS_vX-X_arm
```

### For Daly Windows:
```bash
cd ./Daly/windows
gcc -std=c99 -o EPDataLog_Daly_Windows_vX-X ../../common/getInput.c ../../common/windows/connection.c ../../common/windows/outputToCsv.c ../common/getData.c main.c
```

### For Daly MacOS:
```bash
cd ./Daly/macos
gcc -std=c99 -arch x86_64 -o EPDataLog_Daly_MacOS_vX-X_x86 ../../common/getInput.c ../../common/macos/connection.c ../../common/macos/outputToCsv.c ../common/getData.c main.c

gcc -std=c99 -arch arm64 -o EPDataLog_Daly_MacOS_vX-X_arm ../../common/getInput.c ../../common/macos/connection.c ../../common/macos/outputToCsv.c ../common/getData.c main.c

lipo -create -output EPDataLog_Daly_MacOS_vX-X_universal EPDataLog_Daly_MacOS_vX-X_x86 EPDataLog_Daly_MacOS_vX-X_arm
```

## Version Tagging and Pushing
```
git tag -a EPDataLog_vX.X -m "Released vX.X for all 4 program variants"
git push origin EPDataLog_vX.X
```

## Important Notes
- Whenever a zip file is created for a release, **Make sure the README.md file is bundled in there**. Otherwise, the user won't know how to use the program if they can't access the README on github.
