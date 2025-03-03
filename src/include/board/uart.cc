#include "uart.h"
#include "board/main.h"

namespace fs = std::filesystem;

namespace UART {

// ----------------------------------------
UART::UART(std::string pfx, const std::string dn, const int poll) {
  fd = -1;
  while (fd < 0) {
    while (!dpath.empty()) {
      dpath = prefix_to_path(pfx, dn);
      if (dpath.empty())
        std::this_thread::sleep_for(std::chrono::seconds(poll));
    }
    fd = try_serial_path(this->dpath);
    if (fd < 0)
      std::this_thread::sleep_for(std::chrono::seconds(poll));
  }
}

// ----------------------------------------
std::string UART::prefix_to_path(const std::string pfx, const std::string dn) {
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
int UART::try_serial_path(const std::string path, const speed_t br) {
  int fd = open(path.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
  if (fd < 0) {
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

  cfsetispeed(&tty, br);
  cfsetospeed(&tty, br);

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

// ----------------------------------------
ssize_t UART::write(const uint8_t *dat, const size_t len) {
  if (len > SERIAL_SSIZE_WRITEMAX) {
    std::cerr << "Cannot write length " << len << " greater than maximum "
              << SERIAL_SSIZE_WRITEMAX << std::endl;
    return -1;
  }

  ssize_t b = ::write(fd, dat, len);
  if (b < 0)
    std::cerr << "Write error: " << strerror(errno) << std::endl;
  return b;
}

// ----------------------------------------
ssize_t UART::read(uint8_t *buf, const size_t readmax) {
  if (readmax > SERIAL_SSIZE_READMAX) {
    std::cerr << "Cannot read length " << readmax << " greater than maximum "
              << SERIAL_SSIZE_READMAX << std::endl;
    return -1;
  }

  ssize_t b = ::read(fd, buf, readmax);
  if (b < 0)
    std::cerr << "Read error: " << strerror(errno) << std::endl;
  return b;
}

} // namespace UART
