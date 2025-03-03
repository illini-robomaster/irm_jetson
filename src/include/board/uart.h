#include "main.h"
#include <cerrno>
#include <chrono>
#include <climits>
#include <cstring>
#include <fcntl.h>
#include <filesystem>
#include <iostream>
#include <string>
#include <termios.h>
#include <thread>
#include <unistd.h>

namespace UART {

class UART {
public:
  /**
   * Constructor
   *
   * @param  pfx  Device name prefix
   * @param  dn   Serial dirname
   * @param  poll Rate to retry connection
   * @return      void
   */
  UART(std::string pfx, const std::string dn = SERIAL_DIRNAME,
       const int poll = 1);
  ~UART();
  /**
   * List possible serial port
   *
   * @param  pfx  Device name prefix
   * @param  dn   Serial dirname
   * @return      Path to the device port
   */
  static std::string prefix_to_path(const std::string pfx,
                                    const std::string dn);
  /**
   * Try to open a serial port
   *
   * @param   path  Path to device port
   * @param   br    Baudrate
   * @return        fd on success, -1 otherwise
   */
  static int try_serial_path(const std::string path,
                             const speed_t br = B115200);

  /**
   * Write to fd
   *
   * @param  dat    Packet to send
   * @param  len    Length of packet
   * @return        bytes written on success, -1 otherwise
   */
  ssize_t write(const uint8_t *dat, const size_t len);

  /**
   * Read from fd
   *
   * @param  buffer  Buffer to store received data
   * @return         bytes read on success, 0 on empty, -1 otherwise
   */
  ssize_t read(uint8_t *buf, const size_t readmax);

private:
  std::string dpath;
  int fd;
};

} // namespace UART
