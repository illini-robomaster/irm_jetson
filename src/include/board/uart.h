#include "main.h"
#include <cerrno>
#include <climits>
#include <cstring>
#include <experimental/filesystem>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <termios.h>
#include <thread>
#include <unistd.h>

namespace UARTSERIAL {

class UART {
public:
  /**
   * Constructor
   *
   * @param  fd   Device file descriptor
   * @param  br   Device baudrate
   * @return      void
   */
  UART(int fd, int br = 115200);
  ~UART();

  /**
   * UART object from prefix (defined in main.h)
   *
   * @param  pfx  Device name prefix
   * @param  br   Device baudrate
   * @param  dn   Serial dirname
   * @param  poll Rate to retry connection
   * @return      Pointer to UART object
   */
  static UART *from_prefix(std::string pfx, int br = 115200,
                           const std::string dn = SERIAL_DIRNAME,
                           const int poll = 3);
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
  static int try_serial_path(const std::string path, const int br);

  /**
   * Configure serial termios
   *
   * @param  fd   Device file descriptor
   * @return      fd on success, -1 otherwise
   */
  static int configure(int fd, const int br);

  /**
   * Write to fd
   *
   * @param  dat    Packet to send
   * @param  len    Length of packet
   * @return        bytes written on success, -1 otherwise
   */
  ssize_t write(const uint8_t *dat, const size_t len);

  /**
   * Read from fd (nonblocking)
   *
   * @param  buffer  Buffer to store received data
   * @return         bytes read on success, 0 on empty, -1 otherwise
   */
  ssize_t read(uint8_t *buf, const size_t readmax = SERIAL_SSIZE_READMAX);

private:
  int fd_;
  speed_t br_;
};

} // namespace UARTSERIAL
