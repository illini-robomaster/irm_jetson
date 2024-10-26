#include "can.h"

namespace CANRAW {

Can::Can(string name) {
    s = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (s < 1) {
        std::cerr << "Error while opening socket" << std::endl;
    }

    strcpy(ifr.ifr_name, name);
    ioctl(s, SIOCGIFINDEX, &ifr);

    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        std::cerr << "Error while binding address" << std::endl;
    }
}

void Can::SetTX(canid_t can_id, uint8_t *dat, int len) {
    ftx.can_id = can_id;
    ftx.len = clamp(len, CANXL_MIN_DLEN, CAN_MAX_DLEN);
    memcpy(ftx.data, dat, sizeof(uint8_t) * ftx.can_dlc);
}

void Can::Transmit() {
    if (write(s, &ftx, sizeof(struct can_frame)) !=
            sizeof(struct can_frame)) {
        std::cerr << "Error while sending CAN frame" << std::endl;
    }
}

void Can::Receive() {
    if (read(s, &frx, sizeof(struct can_frame)) !=
            sizeof(struct can_frame)) {
        std::cerr << "Error while receiving CAN frame" << std::endl;
    }
}

void Can::Close() {
    close(s);
}

}
