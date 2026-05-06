#pragma once
#include <stdint.h>
#include <main.h>
#include "fdcan.h"
#include "led.hpp"

class Setting_CAN
{
private:
    FDCAN_RxHeaderTypeDef RxHeader;
    FDCAN_FilterTypeDef RxFilter;
    FDCAN_TxHeaderTypeDef TxHeader;
    LED_Config *led_conf;
    uint8_t RxData[8];

public:
    typedef enum
    {
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

    void init(LED_Config *led_config);
    void send_data(uint32_t can_id, uint8_t *data, uint8_t len);
    void can_callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs);
    void receive_data(uint32_t can_id, uint8_t *data, uint8_t len);
};
