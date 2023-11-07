CC = gcc
CFLAGS = -std=c99
LDFLAGS =
VERSION = vX-X # This is set in the command line when running make

# MacOS build rules
mac: mac_mewyeah mac_daly

mac_mewyeah:
	cd Mewyeah/macos && \
	$(CC) $(CFLAGS) -arch x86_64 -o EPDataLog_Mewyeah_MacOS_$(VERSION)_x86 ../../common/getInput.c ../../common/macos/connection.c ../../common/macos/outputToCsv.c ../common/getData.c main.c && \
	$(CC) $(CFLAGS) -arch arm64 -o EPDataLog_Mewyeah_MacOS_$(VERSION)_arm ../../common/getInput.c ../../common/macos/connection.c ../../common/macos/outputToCsv.c ../common/getData.c main.c && \
	lipo -create -output EPDataLog_Mewyeah_MacOS_$(VERSION)_universal EPDataLog_Mewyeah_MacOS_$(VERSION)_x86 EPDataLog_Mewyeah_MacOS_$(VERSION)_arm && \
	rm EPDataLog_Mewyeah_MacOS_$(VERSION)_x86 EPDataLog_Mewyeah_MacOS_$(VERSION)_arm

mac_daly:
	cd Daly/macos && \
	$(CC) $(CFLAGS) -arch x86_64 -o EPDataLog_Daly_MacOS_$(VERSION)_x86 ../../common/getInput.c ../../common/macos/connection.c ../../common/macos/outputToCsv.c ../common/getData.c main.c && \
	$(CC) $(CFLAGS) -arch arm64 -o EPDataLog_Daly_MacOS_$(VERSION)_arm ../../common/getInput.c ../../common/macos/connection.c ../../common/macos/outputToCsv.c ../common/getData.c main.c && \
	lipo -create -output EPDataLog_Daly_MacOS_$(VERSION)_universal EPDataLog_Daly_MacOS_$(VERSION)_x86 EPDataLog_Daly_MacOS_$(VERSION)_arm && \
	rm EPDataLog_Daly_MacOS_$(VERSION)_x86 EPDataLog_Daly_MacOS_$(VERSION)_arm


# Windows build rules
windows: windows_mewyeah windows_daly

windows_mewyeah:
	cd Mewyeah/windows && \
	$(CC) $(CFLAGS) -o EPDataLog_Mewyeah_Windows_$(VERSION) ../../common/getInput.c ../../common/windows/connection.c ../../common/windows/outputToCsv.c ../common/getData.c main.c

windows_daly:
	cd Daly/windows && \
	$(CC) $(CFLAGS) -o EPDataLog_Daly_Windows_$(VERSION) ../../common/getInput.c ../../common/windows/connection.c ../../common/windows/outputToCsv.c ../common/getData.c main.c

clean:
	rm -f Mewyeah/macos/EPDataLog_Mewyeah_MacOS_$(VERSION)* Daly/macos/EPDataLog_Daly_MacOS_$(VERSION)*
	rm -f Mewyeah/windows/EPDataLog_Mewyeah_Windows_$(VERSION) Daly/windows/EPDataLog_Daly_Windows_$(VERSION)

clean_csv:
	find ./Mewyeah -type f -name '*.csv' -exec rm {} +
	find ./Daly -type f -name '*.csv' -exec rm {} +

.PHONY: mac mac_mewyeah mac_daly windows windows_mewyeah windows_daly clean clean_csv
