#include <unistd.h> 

#include "board/can.h"
#include "motor/motor.h"

int main() {
    CANRAW::CAN *can = new CANRAW::CAN("can0");
    control::MotorCANBase *motor = new control::Motor3508(can, 0x204);
    control::MotorCANBase *motors[] = {motor};

    while (true) {
        motor->SetOutput(800);
        control::MotorCANBase::TransmitOutput(motors, 1);
        usleep(100000);
    }

    return 0;
}
