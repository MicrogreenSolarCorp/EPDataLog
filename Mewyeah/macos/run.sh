#!/bin/bash
# ========================================
# Read data from Daly BMS
# Usage: EPDataLogMewyeahMacOS -t [Interval Time(ms)] -c [Serial Port Number]
# Interval Time: the time interval between two data logs
# Serial Port Number: the Serial port number of the device
# Interval Time and Serial Port Number are optional, default value is 2000 and will autodetect the correct COM Port
# Example: EPDataLogMewyeahMacOS -t 5000 -c 10
# This will log one data every 5000 ms, communicating with the device on /dev/tty.usbserial-10
# ========================================

# Change directory to the script's location
cd "$(dirname "$0")"

# Run the executable with a specific argument
./EPDataLogMewyeahMacOS -t 5000

# Pause (wait for user input)
read -p "Press [Enter] to continue..."
