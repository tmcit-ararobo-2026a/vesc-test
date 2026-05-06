#pragma once
#include <main.h>
#include <stdint.h>

#include "LED.hpp"
#include "fdcan.h"

class SettingCAN
{
private:
    FDCAN_RxHeaderTypeDef rxheader;
    FDCAN_FilterTypeDef rxfilter;
    FDCAN_TxHeaderTypeDef txheader;
    LEDConfig* led_conf;
    uint8_t rxdata[8];

public:
    typedef enum {
        CAN_PACKET_SET_DUTY = 0,
        CAN_PACKET_SET_CURRENT,
        CAN_PACKET_SET_CURRENT_BRAKE,
        CAN_PACKET_SET_RPM,
        CAN_PACKET_SET_POS,
        CAN_PACKET_SET_CURRENT_REL = 10,
        CAN_PACKET_SET_CURRENT_BRAKE_REL,
        CAN_PACKET_SET_CURRENT_HANDBRAKE,
        CAN_PACKET_SET_CURRENT_HANDBRAKE_REL,
        CAN_PACKET_MAKE_ENUM_32_BITS = 0xFFFFFFFF,
    } CAN_PACKET_ID;

    void init(LEDConfig* led_config);
    void send_data(uint32_t can_id, uint8_t* data, uint8_t len);
    void can_callback(FDCAN_HandleTypeDef* hfdcan, uint32_t RxFifo0ITs);
    void receive_data(uint32_t can_id, uint8_t* data, uint8_t len);
};
