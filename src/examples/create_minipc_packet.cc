#include "board/minipc_protocol.h"
#include "board/uart.h"
#include <iostream>

auto minipc_session = communication::MinipcPort();

int main() {
  UART::UART *uart = new UART::UART("placeholder");

  communication::selfcheck_data_t sc_dat;
  uint8_t packet_to_send[minipc_session.MAX_PACKET_LENGTH];

  sc_dat.mode = 1;
  sc_dat.debug_int = 9;

  std::cout << "Creating." << std::endl;

  minipc_session.Pack(packet_to_send, (void *)&sc_dat,
                      communication::GIMBAL_CMD_ID);
  std::cout << "Created." << std::endl;

  return 0;
}
