#include "board/can.h"
#include "motor/motor.h"
#include <unistd.h>

int main() {
  CANRAW::CAN *can = new CANRAW::CAN("can0");
  control::MotorCANBase *motor = new control::Motor3508(can, 0x207);
  control::MotorCANBase *motors[] = {motor};

  std::atomic<bool> *can_stop_flag = can->StartReceiveThread();

  while (true) {
    motor->SetOutput(400);
    control::MotorCANBase::TransmitOutput(motors, 1);
    motor->PrintData();
    usleep(100);
  }

  return 0;
}
