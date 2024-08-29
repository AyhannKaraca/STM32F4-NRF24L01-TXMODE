#ifndef INC_NRF24L01_H_
#define INC_NRF24L01_H_

#include "stm32f4xx_hal.h"
#include "delay.h"
#include "stdbool.h"

//Handler naming
extern SPI_HandleTypeDef hspi1;
#define NRF24_SPI &hspi1

//Port assigning
#define NRF24_CE_PORT   GPIOB
#define NRF24_CE_PIN    GPIO_PIN_1

#define NRF24_CSN_PORT   GPIOB
#define NRF24_CSN_PIN    GPIO_PIN_2


/* Memory Map */
#define CONFIG      0x00
#define EN_AA       0x01
#define EN_RXADDR   0x02
#define SETUP_AW    0x03
#define SETUP_RETR  0x04
#define RF_CH       0x05
#define RF_SETUP    0x06
#define STATUS      0x07
#define OBSERVE_TX  0x08
#define CD          0x09
#define RX_ADDR_P0  0x0A
#define RX_ADDR_P1  0x0B
#define RX_ADDR_P2  0x0C
#define RX_ADDR_P3  0x0D
#define RX_ADDR_P4  0x0E
#define RX_ADDR_P5  0x0F
#define TX_ADDR     0x10
#define RX_PW_P0    0x11
#define RX_PW_P1    0x12
#define RX_PW_P2    0x13
#define RX_PW_P3    0x14
#define RX_PW_P4    0x15
#define RX_PW_P5    0x16
#define FIFO_STATUS 0x17
#define DYNPD	    0x1C
#define FEATURE	    0x1D

/* Instruction Mnemonics */
#define R_REGISTER    0x00
#define W_REGISTER    0x20
#define R_RX_PAYLOAD  0x61
#define W_TX_PAYLOAD  0xA0
#define FLUSH_TX      0xE1
#define FLUSH_RX      0xE2
#define NOP           0xFF

//Enum for pin states
typedef enum {
	LOW = 0,
	HIGH
}NRF24_Pinstate_e;

//Func. Pt's
void CSN_pin (NRF24_Pinstate_e pin_state);
void CE_pin (NRF24_Pinstate_e pin_state);
void NRF24_Write_Register(uint8_t reg, uint8_t data);
void NRF24_Multi_Write_Register(uint8_t reg, uint8_t* data, uint8_t size);
uint8_t NRF24_Read_Register (uint8_t reg);
void NRF24_Multi_Read_Register (uint8_t reg, uint8_t* buff, uint8_t size);
void NRF24_Flush_Tx(void);
void NRF24_Flush_Rx(void);
void Error(void);
bool isOkStart(void);
bool ControlReg_isMatch(uint8_t reg, uint8_t data);
void NRF24_Clear_TX_DS(void);
void NRF24_TxInit(uint8_t* address, uint8_t channel);
void NRF24_Transmit(uint8_t* data);
void NRF24_Reset(void);


#endif /* INC_NRF24L01_H_ */
