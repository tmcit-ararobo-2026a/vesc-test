#include "app/app.hpp"

#include "cstdint"
#include "fdcan.h"
#include "stm32g4xx_hal_fdcan.h"

extern "C" {
// send_function
void can_transmit_eid(uint32_t id, const uint8_t* data, uint8_t len)
{
    FDCAN_TxHeaderTypeDef tx_header;
    tx_header.Identifier          = id;
    tx_header.DataLength          = len;
    tx_header.IdType              = FDCAN_EXTENDED_ID;  // 拡張フォーマットの設定。
    tx_header.TxFrameType         = FDCAN_DATA_FRAME;
    tx_header.FDFormat            = FDCAN_CLASSIC_CAN;
    tx_header.BitRateSwitch       = FDCAN_BRS_OFF;
    tx_header.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    tx_header.TxEventFifoControl  = FDCAN_NO_TX_EVENTS;
    tx_header.MessageMarker       = 0;

    while (HAL_FDCAN_GetTxFifoFreeLevel(&hfdcan3) == 0);
    if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan3, &tx_header, data) != HAL_OK) {
        // HAL_GPIO_WritePin(LED_2_GPIO_Port, LED_2_Pin, GPIO_PIN_SET);
        Error_Handler();
    }
}

// peripheral
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

void buffer_append_int16(uint8_t* buffer, int16_t number, int32_t* index)
{
    buffer[(*index)++] = number >> 8;
    buffer[(*index)++] = number;
}

void buffer_append_int32(uint8_t* buffer, int32_t number, int32_t* index)
{
    buffer[(*index)++] = number >> 24;
    buffer[(*index)++] = number >> 16;
    buffer[(*index)++] = number >> 8;
    buffer[(*index)++] = number;
}

void buffer_append_float16(uint8_t* buffer, float number, float scale, int32_t* index)
{
    buffer_append_int16(buffer, (int16_t)(number * scale), index);
}

void buffer_append_float32(uint8_t* buffer, float number, float scale, int32_t* index)
{
    buffer_append_int32(buffer, (int32_t)(number * scale), index);
}

// set
// controller_id is can_id of VESC
void comm_can_set_duty(uint8_t controller_id, float duty)
{
    int32_t send_index = 0;
    uint8_t buffer[4];
    buffer_append_int32(buffer, (int32_t)(duty * 100000.0), &send_index);
    can_transmit_eid(controller_id | ((uint32_t)CAN_PACKET_SET_DUTY << 8), buffer, send_index);
}

void comm_can_set_current(uint8_t controller_id, float current)
{
    int32_t send_index = 0;
    uint8_t buffer[4];
    buffer_append_int32(buffer, (int32_t)(current * 1000.0), &send_index);
    can_transmit_eid(controller_id | ((uint32_t)CAN_PACKET_SET_CURRENT << 8), buffer, send_index);
}

void comm_can_set_current_off_delay(uint8_t controller_id, float current, float off_delay)
{
    int32_t send_index = 0;
    uint8_t buffer[6];
    buffer_append_int32(buffer, (int32_t)(current * 1000.0), &send_index);
    buffer_append_float16(buffer, off_delay, 1e3, &send_index);
    can_transmit_eid(controller_id | ((uint32_t)CAN_PACKET_SET_CURRENT << 8), buffer, send_index);
}

void comm_can_set_current_brake(uint8_t controller_id, float current)
{
    int32_t send_index = 0;
    uint8_t buffer[4];
    buffer_append_int32(buffer, (int32_t)(current * 1000.0), &send_index);
    can_transmit_eid(
        controller_id | ((uint32_t)CAN_PACKET_SET_CURRENT_BRAKE << 8), buffer, send_index
    );
}

void comm_can_set_rpm(uint8_t controller_id, float rpm)
{
    int32_t send_index = 0;
    uint8_t buffer[4];
    buffer_append_int32(buffer, (int32_t)rpm, &send_index);
    can_transmit_eid(controller_id | ((uint32_t)CAN_PACKET_SET_RPM << 8), buffer, send_index);
}

void comm_can_set_pos(uint8_t controller_id, float pos)
{
    int32_t send_index = 0;
    uint8_t buffer[4];
    buffer_append_int32(buffer, (int32_t)(pos * 1000000.0), &send_index);
    can_transmit_eid(controller_id | ((uint32_t)CAN_PACKET_SET_POS << 8), buffer, send_index);
}

void comm_can_set_current_rel(uint8_t controller_id, float current_rel)
{
    int32_t send_index = 0;
    uint8_t buffer[4];
    buffer_append_float32(buffer, current_rel, 1e5, &send_index);
    can_transmit_eid(
        controller_id | ((uint32_t)CAN_PACKET_SET_CURRENT_REL << 8), buffer, send_index
    );
}

/**
 * Same as above, but also sets the off delay. Note that this command uses 6 bytes now. The off
 * delay is useful to set to keep the current controller running for a while even after setting
 * currents below the minimum current.
 */
void comm_can_set_current_rel_off_delay(uint8_t controller_id, float current_rel, float off_delay)
{
    int32_t send_index = 0;
    uint8_t buffer[6];
    buffer_append_float32(buffer, current_rel, 1e5, &send_index);
    buffer_append_float16(buffer, off_delay, 1e3, &send_index);
    can_transmit_eid(
        controller_id | ((uint32_t)CAN_PACKET_SET_CURRENT_REL << 8), buffer, send_index
    );
}

void comm_can_set_current_brake_rel(uint8_t controller_id, float current_rel)
{
    int32_t send_index = 0;
    uint8_t buffer[4];
    buffer_append_float32(buffer, current_rel, 1e5, &send_index);
    can_transmit_eid(
        controller_id | ((uint32_t)CAN_PACKET_SET_CURRENT_BRAKE_REL << 8), buffer, send_index
    );
}

void comm_can_set_handbrake(uint8_t controller_id, float current)
{
    int32_t send_index = 0;
    uint8_t buffer[4];
    buffer_append_float32(buffer, current, 1e3, &send_index);
    can_transmit_eid(
        controller_id | ((uint32_t)CAN_PACKET_SET_CURRENT_HANDBRAKE << 8), buffer, send_index
    );
}

void comm_can_set_handbrake_rel(uint8_t controller_id, float current_rel)
{
    int32_t send_index = 0;
    uint8_t buffer[4];
    buffer_append_float32(buffer, current_rel, 1e5, &send_index);
    can_transmit_eid(
        controller_id | ((uint32_t)CAN_PACKET_SET_CURRENT_HANDBRAKE_REL << 8), buffer, send_index
    );
}
}
void setup()
{
    HAL_FDCAN_Start(&hfdcan3);
}
void loop()
{
    HAL_GPIO_TogglePin(LED_1_GPIO_Port, LED_1_Pin);
    comm_can_set_current(1, 10);
    HAL_Delay(100);
}