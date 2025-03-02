#include "uart.h"

namespace fs = std::filesystem;

namespace UART {

// ----------------------------------------
UART::UART(std::string pfx, const std::string dn, const int poll) {
  while (!dpath.empty()) {
    dpath = prefix_to_path(pfx, dn);
    std::this_thread::sleep_for(std::chrono::seconds(poll));
  }
  fd = try_serial_path(this->dpath);
}

// ----------------------------------------
std::string UART::prefix_to_path(const std::string pfx, const std::string dn) {
  std::vector<std::string> dev_paths;

  // NOTE: This does not exist when no devices are attached
  if (!fs::exists(SERIAL_DIRNAME)) {
    std::cerr << "Directory " << SERIAL_DIRNAME << " does not exist."
              << std::endl;
    return "";
  }

  try {
    // ls SERIAL_DIRNAME
    for (const auto &entry : fs::directory_iterator(SERIAL_DIRNAME)) {
      std::string filename = entry.path().filename().string();

      // Check against all prefixes
      if (filename.find(pfx) == 0) { // Starts with prefix
        return (fs::path(SERIAL_DIRNAME) / filename).string();
      }
    }
  } catch (const fs::filesystem_error &) {
    std::cerr << "Device with prefix " << pfx << " not found." << std::endl;
  }

  return "";
}

// ----------------------------------------
int UART::try_serial_path(const std::string path, const int br) {
  int fd = open(path.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
  if (fd == -1) {
    std::cerr << "Error getting termios attributes: " << strerror(errno)
              << std::endl;
  }
  // Configure serial port settings
  struct termios tty;
  memset(&tty, 0, sizeof(tty));

  if (tcgetattr(fd, &tty) != 0) {
    std::cerr << "Error getting termios attributes: " << strerror(errno)
              << std::endl;
    close(fd);
    return -1;
  }

  // Set baud rate (input and output)
  speed_t speed;
  switch (br) {
  case 9600:
    speed = B9600;
    break;
  case 19200:
    speed = B19200;
    break;
  case 38400:
    speed = B38400;
    break;
  case 57600:
    speed = B57600;
    break;
  case 115200:
    speed = B115200;
    break;
  default:
    speed = B115200;
  }

  cfsetispeed(&tty, speed);
  cfsetospeed(&tty, speed);

  tty.c_cflag &= ~PARENB;
  tty.c_cflag &= ~CSTOPB;
  tty.c_cflag &= ~CSIZE;
  tty.c_cflag |= CS8;
  tty.c_cflag &= ~CRTSCTS;
  tty.c_cflag |= CREAD | CLOCAL;

  tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
  tty.c_oflag &= ~OPOST;

  if (tcsetattr(fd, TCSANOW, &tty) != 0) {
    std::cerr << "Error setting termios attributes: " << strerror(errno)
              << std::endl;
    close(fd);
    return -1;
  }

  // Flush
  tcflush(fd, TCIOFLUSH);

  return fd;
}

} // namespace UART
