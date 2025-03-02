#include "main.h"
#include <cerrno>
#include <chrono>
#include <cstring>
#include <fcntl.h>
#include <filesystem>
#include <iostream>
#include <string>
#include <termios.h>
#include <thread>
#include <unistd.h>
#include <vector>

namespace UART {

class UART {
public:
  /**
   * Constructor
   *
   * @param pfx  Device name prefix
   * @param dn   Serial dirname
   * @param poll Rate to retry connection
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
  static int try_serial_path(const std::string path, const int br = 115200);

private:
  std::string dpath;
  int fd;
};

} // namespace UART
