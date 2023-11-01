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
4. The first question will ask if the default settings should be used for running the program. This is recommended for most users. Press `y` and `Enter` to continue, then skip to step 7. If custom settings are preferred, type `n` and press `Enter`.
![defaultSettings](https://github.com/MicrogreenSolarCorp/EPDataLog/blob/main/assetsForReadme/defaultSettings.png)
5. If custom settings were chosen, a prompt will appear to enter the COM port number. This is the port number of the RS232-USB cable. The port number can be found in the Device Manager. If unsure of the port number, press `Enter` to perform auto-port-detection.
![portNumber](https://github.com/MicrogreenSolarCorp/EPDataLog/blob/main/assetsForReadme/portNumber.png)
6. If custom settings were chosen, a prompt will appear to enter the data logging interval. This is the interval between each data logging query. The default value is 2000ms. If unsure of what value to enter, press `Enter` to use the default value.
![delayTime](https://github.com/MicrogreenSolarCorp/EPDataLog/blob/main/assetsForReadme/delayTime.png)
7. If this screen appears, the program is running successfully. DO NOT close the command prompt window.
![macosRunSuccess](https://github.com/MicrogreenSolarCorp/EPDataLog/blob/main/assetsForReadme/macosRunSuccess.png)
8. If performing the data logging for a long time (i.e. overnight), ensure that your laptop does not go to sleep and that it has enough battery.


## Sample Output
Refer to [SampleLog.csv](https://github.com/MicrogreenSolarCorp/EPDataLog/blob/main/SampleLog.csv) (Ctrl+Click to open in a new tab) for an example of the output CSV file.

