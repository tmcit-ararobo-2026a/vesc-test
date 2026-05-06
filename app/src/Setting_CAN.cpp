#include "app/Setting_CAN.hpp"

#include <cstring>

// receiveはcan1
void Setting_CAN::init(LED_Config* led_config)
{
    led_conf = led_config;

    HAL_FDCAN_ConfigGlobalFilter(
        &hfdcan3,
        FDCAN_ACCEPT_IN_RX_FIFO0,
        FDCAN_ACCEPT_IN_RX_FIFO0,
        FDCAN_FILTER_REMOTE,
        FDCAN_FILTER_REMOTE
    );

    RxFilter.IdType       = FDCAN_EXTENDED_ID;        // 拡張ID
    RxFilter.FilterIndex  = 0;                        // フィルタインデックス
    RxFilter.FilterType   = FDCAN_FILTER_MASK;        // マスクフィルタ
    RxFilter.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;  // フィルタ設定
    RxFilter.FilterID1    = 0;                        // フィルタID1
    RxFilter.FilterID2    = 0;
    // フィルタ設定
    if (HAL_FDCAN_ConfigFilter(&hfdcan3, &RxFilter) != HAL_OK) {
        Error_Handler();
    }
    if (HAL_FDCAN_Start(&hfdcan3) != HAL_OK) {
        Error_Handler();
    }
    // 割り込み有効
    if (HAL_FDCAN_ActivateNotification(&hfdcan3, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK) {
        Error_Handler();
    }
}

void Setting_CAN::send_data(uint32_t can_id, uint8_t* data, uint8_t len)
{
    TxHeader.Identifier          = can_id;              // canid　拡張も対応
    TxHeader.DataLength          = len;                 // len
    TxHeader.IdType              = FDCAN_EXTENDED_ID;   // 11bit or 27?bit
    TxHeader.TxFrameType         = FDCAN_DATA_FRAME;    // kind of data (this is send data)
    TxHeader.FDFormat            = FDCAN_CLASSIC_CAN;   // Do we use FDCANmode?
    TxHeader.BitRateSwitch       = FDCAN_BRS_OFF;       // BRS change(speed change)
    TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;    // error setting
    TxHeader.TxEventFifoControl  = FDCAN_NO_TX_EVENTS;  // keep a sending history(Yes or No)
    TxHeader.MessageMarker       = 0;                   // complete send messeage

    // wait until TxFIFO free(TxFIFO is 送信待ち行列)
    while (HAL_FDCAN_GetTxFifoFreeLevel(&hfdcan3) == 0);
    if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan3, &TxHeader, data) != HAL_OK) {
        Error_Handler();
    }
}

void Setting_CAN::receive_data(uint32_t can_id, uint8_t* data, uint8_t len)
{
    uint32_t current_id_comp = 43 | ((uint32_t)CAN_PACKET_SET_CURRENT << 8);
    if (can_id == current_id_comp) {
        led_conf->code[0] = data[0];
    }
}

void Setting_CAN::can_callback(FDCAN_HandleTypeDef* hfdcan, uint32_t RxFifo0ITs)
{  // 物理アドレスの参照と新しいメッセージが来たかどうか
    if (hfdcan->Instance == hfdcan3.Instance && RxFifo0ITs == FDCAN_IT_RX_FIFO0_NEW_MESSAGE) {
        if (HAL_FDCAN_GetRxMessage(&hfdcan3, FDCAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK) {
            Error_Handler();
        }
        receive_data(RxHeader.Identifier, RxData, RxHeader.DataLength);
    }
}
