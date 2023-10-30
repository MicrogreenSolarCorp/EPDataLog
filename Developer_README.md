# Developer Readme

## Introduction
EPDataLog is a program designed to facilitate data logging from a Microgreen EnergyPak Battery. At the time of developement, the program supports the following BMS and operating system combinations:
- **Supported BMS:** Mewyeah, Heyo Daly (Also referred to as Daly)
- **Supported Operating Systems:** Windows, MacOS

Notes: 
- it does not support Orion BMS or Linux
- Mewyeah is now a deprecated version of BMS. We have a Mewyeah version to facilaite data logging for older EnergyPaks. All EnergyPaks in the future will be using Heyo Daly BMS.


## File Structure
This is the general file structure of the project:
```
EPDataLog
├── common
│   ├── macos
│   └── windows
│   │   ├── EPDataLogMewyeahWindows.exe
├── Daly
│   ├── common
│   ├── macos
│   └── windows
└── Mewyeah
    ├── common
    ├── macos
    └── windows
```

In the project root (EPDataLog), we have three directories, Daly, Mewyeah, and common. Daly and Mewyeah contain code specific to the respective BMS. Common contains code that is shared between the two BMS versions.

In EPDataLog/common/, we have two directories, macos and windows. These directories contain code that is shared between the two BMS versions. Specifically, each OS directory contains code for:
- connecting to the BMS (MacOS uses serial port communication, Windows uses COM Port communication)
- outputting data to a CSV file (MacOS requires extra configuration to get the correct file path)

In EPDataLog/common/Daly and EPDataLog/common/Mewyeah, we have three directories, common, macos, windows.

### Common
/common contains code that is shared between the two OS versions. Specifically, the common directory contains the code for getting and parsing data from the BMS. The code is the same for both BMS versions, with the exception of reading and writing data to the BMS, in which we use preprocessor conditional directives to include the correct code for each OS version.

### MacOS and Windows
/macos and /windows contains code specific to MacOS and Windows respectively. Specifically, each directory contains the main.c file where the program starts.


## Useful Resources
For instructions on compiling and releasing the code, refer to [Project_Build_And_Release_Instructions.md](https://github.com/MicrogreenSolarCorp/EPDataLog/blob/main/Project_Build_And_Release_Instructions.md)

If changes need to be made to the BMS communication code, refer to the company's internal documentation regarding each BMS.

Refer to [SampleLog.csv](https://github.com/MicrogreenSolarCorp/EPDataLog/blob/main/SampleLog.csv) for an example of the output CSV file.

Refer to Alarm Codes for a list of alarm codes and their meanings.
[Mewyeah Alarm Codes](https://github.com/MicrogreenSolarCorp/EPDataLog/blob/main/Mewyeah/Alarm%20Bit%20Codes.docx)

[Daly Alarm Codes](INSERT LINK HERE)
