CC = gcc
CFLAGS = -std=c99
LDFLAGS =
VERSION = vX-X # This is set in the command line when running make

# MacOS build rules
mac: mac_mewyeah mac_daly mac_orion

mac_mewyeah:
	cd Mewyeah/macos && \
	$(CC) $(CFLAGS) -arch x86_64 -o EPDataLog_Mewyeah_MacOS_$(VERSION)_x86 ../../common/getInput.c ../../common/macos/connection.c ../../common/macos/outputToCsv.c ../common/getData.c main.c && \
	$(CC) $(CFLAGS) -arch arm64 -o EPDataLog_Mewyeah_MacOS_$(VERSION)_arm ../../common/getInput.c ../../common/macos/connection.c ../../common/macos/outputToCsv.c ../common/getData.c main.c && \
	lipo -create -output EPDataLog_Mewyeah_MacOS_$(VERSION)_universal EPDataLog_Mewyeah_MacOS_$(VERSION)_x86 EPDataLog_Mewyeah_MacOS_$(VERSION)_arm && \
	rm EPDataLog_Mewyeah_MacOS_$(VERSION)_x86 EPDataLog_Mewyeah_MacOS_$(VERSION)_arm
	cd Mewyeah/macos && zip -j EPDataLog_Mewyeah_MacOS_$(VERSION).zip EPDataLog_Mewyeah_MacOS_$(VERSION)_universal ../../README.md

mac_daly:
	cd Daly/macos && \
	$(CC) $(CFLAGS) -arch x86_64 -o EPDataLog_Daly_MacOS_$(VERSION)_x86 ../../common/getInput.c ../../common/macos/connection.c ../../common/macos/outputToCsv.c ../common/getData.c main.c && \
	$(CC) $(CFLAGS) -arch arm64 -o EPDataLog_Daly_MacOS_$(VERSION)_arm ../../common/getInput.c ../../common/macos/connection.c ../../common/macos/outputToCsv.c ../common/getData.c main.c && \
	lipo -create -output EPDataLog_Daly_MacOS_$(VERSION)_universal EPDataLog_Daly_MacOS_$(VERSION)_x86 EPDataLog_Daly_MacOS_$(VERSION)_arm && \
	rm EPDataLog_Daly_MacOS_$(VERSION)_x86 EPDataLog_Daly_MacOS_$(VERSION)_arm
	cd Daly/macos && zip -j EPDataLog_Daly_MacOS_$(VERSION).zip EPDataLog_Daly_MacOS_$(VERSION)_universal ../../README.md

mac_orion:
	cd Orion/macos && \
	$(CC) $(CFLAGS) -arch x86_64 -o EPDataLog_Orion_MacOS_$(VERSION)_x86 ../../common/getInput.c ../../common/macos/connection.c ../../common/macos/outputToCsv.c ../common/getData.c main.c && \
	$(CC) $(CFLAGS) -arch arm64 -o EPDataLog_Orion_MacOS_$(VERSION)_arm ../../common/getInput.c ../../common/macos/connection.c ../../common/macos/outputToCsv.c ../common/getData.c main.c && \
	lipo -create -output EPDataLog_Orion_MacOS_$(VERSION)_universal EPDataLog_Orion_MacOS_$(VERSION)_x86 EPDataLog_Orion_MacOS_$(VERSION)_arm && \
	rm EPDataLog_Orion_MacOS_$(VERSION)_x86 EPDataLog_Orion_MacOS_$(VERSION)_arm
	cd Orion/macos && zip -j EPDataLog_Orion_MacOS_$(VERSION).zip EPDataLog_Orion_MacOS_$(VERSION)_universal ../../README.md

# Windows build rules
windows: windows_mewyeah windows_daly windows_orion

windows_mewyeah:
	cd Mewyeah/windows && \
	$(CC) $(CFLAGS) -o EPDataLog_Mewyeah_Windows_$(VERSION) ../../common/getInput.c ../../common/windows/connection.c ../../common/windows/outputToCsv.c ../common/getData.c main.c && \
	PowerShell -Command "Compress-Archive -Path EPDataLog_Mewyeah_Windows_$(VERSION).exe, ../../README.md -DestinationPath EPDataLog_Mewyeah_Windows_$(VERSION).zip"

windows_daly:
	cd Daly/windows && \
	$(CC) $(CFLAGS) -o EPDataLog_Daly_Windows_$(VERSION) ../../common/getInput.c ../../common/windows/connection.c ../../common/windows/outputToCsv.c ../common/getData.c main.c && \
	PowerShell -Command "Compress-Archive -Path EPDataLog_Daly_Windows_$(VERSION).exe, ../../README.md -DestinationPath EPDataLog_Daly_Windows_$(VERSION).zip"

windows_orion:
	cd Orion/windows && \
	$(CC) $(CFLAGS) -o EPDataLog_Orion_Windows_$(VERSION) ../../common/getInput.c ../../common/windows/connection.c ../../common/windows/outputToCsv.c ../common/getData.c main.c && \
	PowerShell -Command "Compress-Archive -Path EPDataLog_Orion_Windows_$(VERSION).exe, ../../README.md -DestinationPath EPDataLog_Orion_Windows_$(VERSION).zip"

# Rules for cleanup
clean_windows:
	cmd /C "del /Q Daly\windows\EPDataLog_Daly_Windows_*"
	cmd /C "del /Q Mewyeah\windows\EPDataLog_Mewyeah_Windows_*"
	cmd /C "del /Q Orion\windows\EPDataLog_Orion_Windows_*"

clean_mac:
	rm -f Mewyeah/macos/EPDataLog_Mewyeah_MacOS_* Daly/macos/EPDataLog_Daly_MacOS_* Orion/macos/EPDataLog_Orion_MacOS_*

clean_csv:
    ifeq ($(OS),Darwin) # macOS
		find ./Mewyeah -type f -name '*.csv' -exec rm {} +
		find ./Daly -type f -name '*.csv' -exec rm {} +
		find ./Orion -type f -name '*.csv' -exec rm {} +
    else ifeq ($(OS),Linux) # Linux
    else # Assuming Windows
		PowerShell -Command "Get-ChildItem ./Mewyeah -Recurse -Filter *.csv | Remove-Item"
		PowerShell -Command "Get-ChildItem ./Daly -Recurse -Filter *.csv | Remove-Item"
		PowerShell -Command "Get-ChildItem ./Orion -Recurse -Filter *.csv | Remove-Item"
    endif


.PHONY: mac mac_mewyeah mac_daly mac_orion windows windows_mewyeah windows_daly windows_orion clean_windows clean_macos clean_csv
