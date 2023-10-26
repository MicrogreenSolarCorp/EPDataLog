--------------------------------------------------------------
                      README.txt
--------------------------------------------------------------

Batch File for Mewyeah BMS Data Logging on Microgreen EnergyPak Battery
===================================================================

Description
-----------
This batch file is designed to facilitate data logging from a Microgreen EnergyPak Battery equipped with a Mewyeah Battery Management System (BMS). The script employs the "EPDataLogMewyeahWindows.exe" executable, offering options to customize the time interval between logs and the COM port for device communication. 

The program generates a .csv file, naming it according to the current timestamp. As long as the program window remains open, it will continuously read and log data from the BMS to this .csv file.

Note: This program is Windows-specific and is not compatible with MacOS.



Quickstart
----------
For quick setup and default settings, simply double-click the "run.bat" file. This action will initiate data logging with an auto-detected COM port and a default read interval of 5 seconds (5000 ms).



Custom Configuration
--------------------
To modify the read interval or specify a COM port:

1. Open the "run.bat" file using Notepad. To do this, open the Notepad application and navigate to File >> Open, or right-click on "run.bat" and select "Open with" >> "Notepad".
2. In the batch file, locate the line with the command "EPDataLogMewyeah.exe -t [Interval Time(ms)] -c [COM Port Number]".
3. Replace "[Interval Time(ms)]" and "[COM Port Number]" with your desired values.
4. Save the modifications to the "run.bat" file. You can quickly save by pressing Ctrl + S.
5. Double-click the updated "run.bat" file to initiate the data logging process.

Parameters
----------
- "-t [Interval Time(ms)]": Specifies the time interval in milliseconds between two consecutive data logs. The default value is 5000 ms.
  
- "-c [COM Port Number]": Specifies the COM port number for device communication. If not specified, the script will auto-detect the correct COM port.

Example
-------
To perform data logging every 5000 ms through COM3, modify the batch file to include:

  EPDataLogMewyeahWindows.exe -t 5000 -c 3

--------------------------------------------------------------
