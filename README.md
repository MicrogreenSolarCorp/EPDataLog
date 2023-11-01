# EPDataLog

## Description

EPDataLog is a program designed to facilitate data logging from a Microgreen EnergyPak Battery.
- **Supported BMS:** Mewyeah, Heyo Daly
- **Supported Operating Systems:** Windows, MacOS

## Installation

1. If you are on Windows, ensure you have the [CH340 Driver](https://learn.sparkfun.com/tutorials/how-to-install-ch340-drivers/all){:target="_blank"} installed. Scroll down under the Windows 7/10 header and click on CH341SER (EXE).This driver is required for the RS232-USB cable to work correctly. If you are on MacOS, the driver should be installed automatically.
2. Navigate to the [release page](https://github.com/MicrogreenSolarCorp/EPDataLog/releases){:target="_blank"}.
3. Select the latest release and expand the Assets dropdown menu.
4. **Important:** Ensure you know the BMS version that your EnergyPak is using (Mewyeah or Daly). Download the corresponding zip file for your operating system and BMS version.
5. Unzip the downloaded file.

## Usage

### Windows

#### Quickstart
1. Make sure your EnergyPak is ON and connected to your computer via the RS232-USB cable. If you are using a 12kWh EnergyPak, make sure the display is turned OFF.
2. Simply open the unzipped folder and double-click on the `run.bat` file to start data logging with default settings.

![windowsRun](https://github.com/MicrogreenSolarCorp/EPDataLog/blob/main/assetsForReadme/windowsRun.png){:target="_blank"}
![windowsRunSuccess](https://github.com/MicrogreenSolarCorp/EPDataLog/blob/main/assetsForReadme/windowsRunSuccess.png){:target="_blank"}

#### Custom Configuration
If you would like to modify the read interval or specify a COM port:
1. Right-click on `run.bat` and select "Open with" → "Notepad", or open Notepad and navigate to File → Open to select `run.bat`.
2. Locate the line with the command `EPDataLogMewyeah.exe -t [Interval Time(ms)] -c [COM Port Number]`.
3. Replace `[Interval Time(ms)]` and `[COM Port Number]` with your desired values.
4. Save the modifications to the `run.bat` file (Ctrl + S).
5. Double-click the updated `run.bat` file to initiate the data logging process.

### MacOS

#### Quickstart
1. Make sure your EnergyPak is ON and connected to your computer via the RS232-USB cable. If you are using a 12kWh EnergyPak, make sure the display is turned OFF.
2. Open the folder and double-click on the program file to start data logging with default settings.

![macosRun](https://github.com/MicrogreenSolarCorp/EPDataLog/blob/main/assetsForReadme/macosRun.png){:target="_blank"}
![macosRunSuccess](https://github.com/MicrogreenSolarCorp/EPDataLog/blob/main/assetsForReadme/macosRunSuccess.png){:target="_blank"}

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
Refer to [SampleLog.csv](https://github.com/MicrogreenSolarCorp/EPDataLog/blob/main/SampleLog.csv){:target="_blank"} for an example of the output CSV file.

