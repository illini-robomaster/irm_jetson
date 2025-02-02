#include "stdio.h"
#include <unistd.h>
#include <iostream>
#include "stdint.h"
#include "board/can.h"

void receive(CANRAW::CAN *can0)
{
  // CANRAW::CAN *can0 = new CANRAW::CAN("can0");
  // read(, &can0->frx, sizeof(struct can_frame));
  can0->Receive();
  for (int i = 0; i < 8; i++)
  {
    printf("%02x ", (int)can0->frx.data[i]);
  }
  printf("\n");
}

int main()
{
  CANRAW::CAN *can0 = new CANRAW::CAN("can0");
  printf("Start can receive test\n");
  while (true)
  {
    receive(can0);
  }
  return 0;
}