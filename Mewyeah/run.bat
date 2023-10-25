REM ========================================
REM Read data from Daly BMS
REM Usage: EPDataLogMewyeah.exe -t [Interval Time(ms)] -c [COM Port Number]
REM Interval Time: the time interval between two data logs
REM COM Port Number: the COM port number of the device
REM Interval Time and COM Port are optional, default value is 2000 and will autodetect the correct COM Port
REM Example: EPDataLogMewyeah.exe -t 5000 -c 3
REM this is log one data every 5000 ms, communicating via COM3
REM ========================================

cd /d %~dp0

"%~dp0EPDataLogMewyeah.exe" -t 5000
pause