#include "app/SettingCAN.hpp"

#include <cstring>

// receiveはcan1
void SettingCAN::init(LEDConfig* led_config)
{
    led_conf = led_config;

    HAL_FDCAN_ConfigGlobalFilter(
        &hfdcan3,
        FDCAN_ACCEPT_IN_RX_FIFO0,
        FDCAN_ACCEPT_IN_RX_FIFO0,
        FDCAN_FILTER_REMOTE,
        FDCAN_FILTER_REMOTE
    );

    rxfilter.IdType       = FDCAN_EXTENDED_ID;        // 拡張ID
    rxfilter.FilterIndex  = 0;                        // フィルタインデックス
    rxfilter.FilterType   = FDCAN_FILTER_MASK;        // マスクフィルタ
    rxfilter.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;  // フィルタ設定
    rxfilter.FilterID1    = 0;                        // フィルタID1
    rxfilter.FilterID2    = 0;
    // フィルタ設定
    if (HAL_FDCAN_ConfigFilter(&hfdcan3, &rxfilter) != HAL_OK) {
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

void SettingCAN::send_data(uint32_t can_id, uint8_t* data, uint8_t len)
{
    txheader.Identifier          = can_id;              // canid　拡張も対応
    txheader.DataLength          = len;                 // len
    txheader.IdType              = FDCAN_EXTENDED_ID;   // 11bit or 27?bit
    txheader.TxFrameType         = FDCAN_DATA_FRAME;    // kind of data (this is send data)
    txheader.FDFormat            = FDCAN_CLASSIC_CAN;   // Do we use FDCANmode?
    txheader.BitRateSwitch       = FDCAN_BRS_OFF;       // BRS change(speed change)
    txheader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;    // error setting
    txheader.TxEventFifoControl  = FDCAN_NO_TX_EVENTS;  // keep a sending history(Yes or No)
    txheader.MessageMarker       = 0;                   // complete send messeage

    // wait until TxFIFO free(TxFIFO is 送信待ち行列)
    while (HAL_FDCAN_GetTxFifoFreeLevel(&hfdcan3) == 0);
    if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan3, &txheader, data) != HAL_OK) {
        Error_Handler();
    }
}

void SettingCAN::receive_data(uint32_t can_id, uint8_t* data, uint8_t len)
{
    uint32_t current_id_comp = 43 | ((uint32_t)CAN_PACKET_SET_CURRENT << 8);
    if (can_id == current_id_comp) {
        led_conf->code[0] = data[0];
    }
}

void SettingCAN::can_callback(FDCAN_HandleTypeDef* hfdcan, uint32_t RxFifo0ITs)
{  // 物理アドレスの参照と新しいメッセージが来たかどうか
    if (hfdcan->Instance == hfdcan3.Instance && RxFifo0ITs == FDCAN_IT_RX_FIFO0_NEW_MESSAGE) {
        if (HAL_FDCAN_GetRxMessage(&hfdcan3, FDCAN_RX_FIFO0, &rxheader, rxdata) != HAL_OK) {
            Error_Handler();
        }
        receive_data(rxheader.Identifier, rxdata, rxheader.DataLength);
    }
}
