#include <cstring>
#include <stdint.h>
#include <unistd.h>
#include <iostream>

#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

namespace CANRAW {

template<class T>
constexpr const T& clamp(const T& v, const T& lo, const T& hi)
{
    return v < lo ? lo : hi < v ? hi : v;
}

class CAN {
  public:
    CAN(const char* name = "can0");
    void Transmit(canid_t can_id, uint8_t *dat, int len);
    void Receive();
    void Close();
    struct can_frame frx;
  private:
    int s;
    struct sockaddr_can addr;
    struct ifreq ifr;
    struct can_frame ftx;
};

}
