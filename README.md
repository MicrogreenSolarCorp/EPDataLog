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
![windowsRun](https://github.com/MicrogreenSolarCorp/EPDataLog/blob/main/assetsForReadme/windowsRun.png)
3. A command prompt window will open. Follow the instructions highlighted in red. 
4. The first question will ask if you would like to run the program with default settings. This is recommended for most users. Press `y` and `Enter` to continue, then skip to step 7. If you would like to customize the settings, type `n` and press `Enter`.
5. If you chose to customize the settings, you will be prompted to enter the COM port number. This is the port number of the RS232-USB cable. The port number can be found in the Device Manager. If you are unsure of the port number, press `Enter` to perform auto-port-detection.
6. If you chose to customize the settings, you will be prompted to enter the data logging interval. This is the interval between each data logging query. The default value is 2000ms. If you are unsure of what value to enter, press `Enter` to use the default value.

7. If you see this screen, the program is running successfully. DO NOT close the command prompt window.
![macRunSuccess](https://github.com/MicrogreenSolarCorp/EPDataLog/blob/main/assetsForReadme/macRunSuccess.png)


## Sample Output
Refer to [SampleLog.csv](https://github.com/MicrogreenSolarCorp/EPDataLog/blob/main/SampleLog.csv) (Ctrl+Click to open in a new tab) for an example of the output CSV file.

