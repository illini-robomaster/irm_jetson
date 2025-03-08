#include "board/main.h"
#include "board/minipc_protocol.h"
#include "board/uart.h"
#include <iostream>

auto minipc_session = communication::MinipcPort();

int main() {
  UARTSERIAL::UART *uart = UARTSERIAL::UART::from_prefix(PREF_CBOARD, 115200);

  communication::selfcheck_data_t sc_dat;
  uint8_t txdat[minipc_session.MAX_PACKET_LENGTH];
  uint8_t *rxdat;

  sc_dat.mode = 1;
  sc_dat.debug_int = 9;

  std::cout << "Creating packed." << std::endl;
  minipc_session.Pack(txdat, (void *)&sc_dat, communication::GIMBAL_CMD_ID);

  std::cout << "Sending packet." << std::endl;
  uart->write(txdat, minipc_session.GetPacketLen(communication::GIMBAL_CMD_ID));

  std::cout << "Reading packet." << std::endl;
  uart->read(rxdat);

  return 0;
}
