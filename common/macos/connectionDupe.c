#include "connection.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>

// Global Vars
int g_delay_time_ms = 2000;


int setupSerialPort(const char *device, int baudRate) {
    int fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1) {
        perror("open_port: Unable to open the serial port");
        return -1;
    }

    struct termios options;
    tcgetattr(fd, &options);

    // Set the baud rate
    cfsetispeed(&options, baudRate);
    cfsetospeed(&options, baudRate);

    // Set 8N1 mode
    options.c_cflag &= ~PARENB; // No parity
    options.c_cflag &= ~CSTOPB; // 1 stop bit
    options.c_cflag &= ~CSIZE;  // Mask the character size bits
    options.c_cflag |= CS8;     // 8 data bits

    // Enable the receiver and set local mode
    options.c_cflag |= (CLOCAL | CREAD);

    // Set the new options for the port
    tcsetattr(fd, TCSANOW, &options);

    return fd;
}
