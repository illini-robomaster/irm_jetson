#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>

int main()
{
  int fd = open("/dev/ttyTHS0", O_RDWR | O_NOCTTY | O_NDELAY); // For Jetson Nano
  if (fd == -1)
  {
    std::cerr << "Error opening UART port" << std::endl;
    return -1;
  }
  struct termios options;
  tcgetattr(fd, &options);

  // Set baud rate
  cfsetispeed(&options, B115200);
  cfsetospeed(&options, B115200);

  // Set 8 data bits, no parity, 1 stop bit
  options.c_cflag &= ~PARENB;
  options.c_cflag &= ~CSTOPB;
  options.c_cflag &= ~CSIZE;
  options.c_cflag |= CS8;

  // Set raw input and output
  options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
  options.c_oflag &= ~OPOST;

  tcsetattr(fd, TCSANOW, &options);

  // Write data
  const char *message = "Hello from Jetson!";
  int bytes_written = write(fd, message, strlen(message));
  if (bytes_written < 0)
  {
    std::cerr << "Error writing to UART" << std::endl;
  }
  while (1)
  {
    // Write data
    const char *message = "Hello from Jetson!";
    int bytes_written = write(fd, message, strlen(message));
    if (bytes_written < 0)
    {
      std::cerr << "Error writing to UART" << std::endl;
    }
    usleep(1000000);
  }

  close(fd);
  return 0;
}