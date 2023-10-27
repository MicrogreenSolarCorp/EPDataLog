#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>

#include <signal.h>

volatile sig_atomic_t done = 0;

void signal_handler(int sig) {
    done = 1;
}


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



int main() {
	signal(SIGINT, signal_handler);

    const char *device = "/dev/tty.usbserial-10"; // Replace with your device name
    int baudRate = B38400; // Replace with your desired baud rate

    int fd = setupSerialPort(device, baudRate);
    if (fd == -1) {
        return 1;
    }

    unsigned char connectionQueryData[] = {0XEB, 0X90, 0X1F, 0X04, 0X07, 0x9A, 0x00, 0x00, 0x79};

    ssize_t bytes_written = write(fd, connectionQueryData, sizeof(connectionQueryData));
    if (bytes_written < 0) {
        perror("Error writing to serial port");
        close(fd);
        return 1;
    } else {
        printf("Successfully sent: ");
        for (size_t i = 0; i < sizeof(connectionQueryData); ++i) {
            printf("%02X ", connectionQueryData[i]);
        }
        printf("\n");
    }

    unsigned char buffer[300];

	while (!done) {
        int n = read(fd, buffer, sizeof(buffer));
        if (n < 0) {
            if (errno != EAGAIN) {
                perror("Error reading from serial port");
                break;
            }
        } else if (n > 0) {
            printf("Received %d bytes: ", n);
            for (int i = 0; i < n; ++i) {
                printf("%02X ", buffer[i]);
            }
            printf("\n");
        } else {
            usleep(100000);  // Sleep for 100 ms to reduce CPU usage, adjust as necessary
        }
    }
    
    printf("Exiting...\n");
    close(fd);
    return 0;
}
