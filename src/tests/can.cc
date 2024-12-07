#include "stdio.h"
#include "stdint.h"
#include "board/can.h"

void sendtest() {
    int i;
    int len = 8;
    uint8_t dat[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00};

    CANRAW::CAN *can0 = new CANRAW::CAN("can0");

    for (i = 0; i < 10; i++) {
        can0->Transmit(0x200, dat, len);
    }
    can0->Close();
}

int main() {
    std::cout << "Start can send test" << std::endl;
    sendtest();
    std::cout << "End can send test" << std::endl;

    return 0;
}
