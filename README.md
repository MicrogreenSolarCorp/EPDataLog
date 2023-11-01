# EPDataLog

## Description

EPDataLog is a program designed to facilitate data logging from a Microgreen EnergyPak Battery.
- **Supported BMS:** Mewyeah, Heyo Daly
- **Supported Operating Systems:** Windows, MacOS

## Installation

1. On Windows, install the [CH340 Driver](https://www.wch-ic.com/downloads/CH341SER_ZIP.html) (Ctrl+Click to open in a new tab). This step can be ignored if on MacOS.
2. Navigate to the [release page](https://github.com/MicrogreenSolarCorp/EPDataLog/releases) (Ctrl+Click to open in a new tab).
3. **Important:** Check the BMS version of the EnergyPak. It should be either Mewyeah or Daly. The BMS version can be found on the product label of the EnergyPak. 
4. Download the corresponding zip file for the operating system and BMS version.
5. Unzip the downloaded file. This step can be ignored if on MacOS.

## Usage

### Windows
1. Ensure the EnergyPak is ON and connected to the computer via the RS232-USB cable. If using a 12kWh EnergyPak, ensure the display is turned OFF.
2. Open the unzipped folder and double-click on the `EPDataLog_Mewyeah_Windows_v1-2.exe` or `EPDataLog_Daly_Windows_v1-2.exe` file.
3. A command prompt window will open. Follow the instructions highlighted in red. 



#### Quickstart
1. Ensure the EnergyPak is ON and connected to the computer via the RS232-USB cable. If using a 12kWh EnergyPak, ensure the display is turned OFF.
2. Open the folder and double-click on the program file to start data logging with default settings.

![macosRun](https://github.com/MicrogreenSolarCorp/EPDataLog/blob/main/assetsForReadme/macosRun.png)
![macosRunSuccess](https://github.com/MicrogreenSolarCorp/EPDataLog/blob/main/assetsForReadme/macosRunSuccess.png)

### Custom Configuration Instructions

#### Parameters
- `-t [Interval Time(ms)]`: Specifies the time interval in milliseconds between two consecutive data logs. The default value is 5000 ms.
- `-c [COM Port Number]`: Specifies the COM port number for device communication. If not specified, the script will auto-detect the correct COM port.

#### Custom Configuration Example
To perform data logging every 5000 ms through COM3, modify the batch file to include:
```bash
EPDataLogMewyeahWindows.exe -t 5000 -c 3
```

## Sample Output
Refer to [SampleLog.csv](https://github.com/MicrogreenSolarCorp/EPDataLog/blob/main/SampleLog.csv) (Ctrl+Click to open in a new tab) for an example of the output CSV file.

