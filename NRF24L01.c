
#include "NRF24L01.h"


//Set CSN pin

void CSN_pin (NRF24_Pinstate_e pin_state){

	if(pin_state){
		HAL_GPIO_WritePin(NRF24_CSN_PORT, NRF24_CSN_PIN, GPIO_PIN_SET);
	}
	else{
		HAL_GPIO_WritePin(NRF24_CSN_PORT, NRF24_CSN_PIN, GPIO_PIN_RESET);
	}
}
//Set CE pin

void CE_pin (NRF24_Pinstate_e pin_state){

	if(pin_state){
		HAL_GPIO_WritePin(NRF24_CE_PORT, NRF24_CE_PIN, GPIO_PIN_SET);
	}
	else{
		HAL_GPIO_WritePin(NRF24_CE_PORT, NRF24_CE_PIN, GPIO_PIN_RESET);
	}
}

/*	This function checks whether the data intended to be written to the registers is correctly written,
except for the FIFO_STATUS and STATUS registers. This is because the situation is different for these registers.
For example, if the TX_DS bit of the STATUS register is set, writing 1 to this bit will reset it.
So, you write 1 to the TX_DS bit, but it becomes 0. In other words, what you write is not the same as what you read.
Therefore, we do not check these registers.	*/

bool ControlReg_isMatch(uint8_t reg, uint8_t data){

	if(reg == STATUS || reg == FIFO_STATUS){
		return true;
	}

	if(NRF24_Read_Register(reg) != data){
		return false;
	}
	else{
		return true;
	}
}



void NRF24_Write_Register(uint8_t reg, uint8_t data){

	uint8_t cmd = W_REGISTER;
	cmd |= reg;
	CE_pin(LOW);
	CSN_pin(LOW);
	HAL_SPI_Transmit(NRF24_SPI, &cmd, 1, 1000);
	HAL_SPI_Transmit(NRF24_SPI, &data, 1, 1000);
	CSN_pin(HIGH);

	delay_us(100);

	if(!(ControlReg_isMatch(reg, data))){
		Error();
	}

}

void NRF24_Multi_Write_Register(uint8_t reg, uint8_t* data, uint8_t size){

	uint8_t cmd = W_REGISTER;
	cmd |= reg;
	CE_pin(LOW);
	CSN_pin(LOW);
	HAL_SPI_Transmit(NRF24_SPI, &cmd, 1, 1000);
	HAL_SPI_Transmit(NRF24_SPI, data, size, 1000);
	CSN_pin(HIGH);

}


uint8_t NRF24_Read_Register (uint8_t reg){

	uint8_t data = 0;
	uint8_t cmd = R_REGISTER;
	cmd |= reg;
	CE_pin(LOW);
	CSN_pin(LOW);
	HAL_SPI_Transmit(NRF24_SPI, &cmd, 1, 1000);
	HAL_SPI_Receive(NRF24_SPI, &data, 1, 1000);
	CSN_pin(HIGH);
	return data;
}


void NRF24_Multi_Read_Register (uint8_t reg, uint8_t* buff, uint8_t size){

	uint8_t cmd = R_REGISTER;
	cmd |= reg;
	CE_pin(LOW);
	CSN_pin(LOW);
	HAL_SPI_Transmit(NRF24_SPI, &cmd, 1, 1000);
	HAL_SPI_Receive(NRF24_SPI, buff, size, 1000);
	CSN_pin(HIGH);
}


void NRF24_Flush_Tx(void){
	uint8_t cmd = FLUSH_TX;
	CE_pin(LOW);
	CSN_pin(LOW);
	HAL_SPI_Transmit(NRF24_SPI, &cmd, 1, 1000);
	cmd = NOP;
	HAL_SPI_Transmit(NRF24_SPI, &cmd, 1, 1000);
	CSN_pin(HIGH);
}

void NRF24_Flush_Rx(void){
	uint8_t cmd = FLUSH_RX;
	CE_pin(LOW);
	CSN_pin(LOW);
	HAL_SPI_Transmit(NRF24_SPI, &cmd, 1, 1000);
	cmd = NOP;
	HAL_SPI_Transmit(NRF24_SPI, &cmd, 1, 1000);
	CSN_pin(HIGH);
}

void NRF24_Clear_TX_DS(void){

		NRF24_Write_Register(STATUS, (1<<5));
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);//Status led.

}

/*	This function is called when unwanted situations occur in certain parts of the program.
	Decide what needs to be done in this function based on your needs.	*/

void Error(void){
	while(1){
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
		HAL_Delay(500);
	}
}

/*	As it is known, there is no reset pin on the chip. Therefore, you can manually reset it by cutting
 *	off the power for a short period. You might encounter situations where you need to reset the chip.
 *	This function uses the PC1 pin as a switch and sets it to LOW for 200ms, cutting off the module's power,
 *	and then reapplying the power. This way, you reset the module.	*/

void NRF24_Reset(void){

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, RESET);
	HAL_Delay(200);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, SET);
	HAL_Delay(150);
	CE_pin(LOW);
	uint8_t cmd = NOP;
	//These lines were written to wake up this chip. In my case, this lines were looking necessary.
	CSN_pin(LOW);
	HAL_SPI_Transmit(NRF24_SPI, &cmd, 1, 100);
	CSN_pin(HIGH);

}

/*	A function that checks whether the registers are at their default values.	*/

bool isOkStart(void){

	NRF24_Reset();


	if(NRF24_Read_Register(CONFIG)!=0x08){
			return false;
	}
	if(NRF24_Read_Register(EN_AA)!=0x3F){
			return false;
		}
	if(NRF24_Read_Register(EN_RXADDR)!=0x03){
			return false;
		}
	if(NRF24_Read_Register(SETUP_AW)!=0x03){
			return false;
		}
	if(NRF24_Read_Register(SETUP_RETR)!=0x03){
			return false;
		}
	if(NRF24_Read_Register(RF_CH)!=0x02){
			return false;
		}
	if(NRF24_Read_Register(RF_SETUP)!=0x0F){
			return false;
		}
	if(NRF24_Read_Register(STATUS)!=0x0E){
			return false;
		}
	if(NRF24_Read_Register(OBSERVE_TX)!=0x00){
			return false;
		}
	if(NRF24_Read_Register(CD)!=0x00){
			return false;
		}
	if(NRF24_Read_Register(RX_ADDR_P0)!=0xE7){
			return false;
		}
	if(NRF24_Read_Register(RX_ADDR_P1)!=0xC2){
			return false;
		}
	if(NRF24_Read_Register(RX_ADDR_P2)!=0xC3){
			return false;
		}
	if(NRF24_Read_Register(RX_ADDR_P3)!=0xC4){
			return false;
		}
	if(NRF24_Read_Register(RX_ADDR_P4)!=0xC5){
			return false;
		}
	if(NRF24_Read_Register(RX_ADDR_P5)!=0xC6){
			return false;
		}
	if(NRF24_Read_Register(TX_ADDR)!=0xE7){
			return false;
		}
	if(NRF24_Read_Register(RX_PW_P0)!=0x00){
			return false;
		}
	if(NRF24_Read_Register(RX_PW_P1)!=0x00){
			return false;
		}
	if(NRF24_Read_Register(RX_PW_P2)!=0x00){
			return false;
		}
	if(NRF24_Read_Register(RX_PW_P3)!=0x00){
			return false;
		}
	if(NRF24_Read_Register(RX_PW_P4)!=0x00){
			return false;
		}
	if(NRF24_Read_Register(RX_PW_P5)!=0x00){
			return false;
		}
	if(NRF24_Read_Register(FIFO_STATUS)!=0x11){
			return false;
		}
	if(NRF24_Read_Register(DYNPD)!=0x00){
			return false;
		}
	if(NRF24_Read_Register(FEATURE)!=0x00){
			return false;
		}

	return true;

}


/* TX init function configured with the most basic features	*/

void NRF24_TxInit(uint8_t* address, uint8_t channel){

	CE_pin(LOW);
	uint8_t data = 0;

	//we wont use the IRQ pin. So we can mask these bits.
	data = 0b01110000;
	NRF24_Write_Register(CONFIG, data);
	//we wont use Enhanced ShockBurst
	data = 0;
	NRF24_Write_Register(EN_AA, data);
	//EN_RXADDR will be set in RX mode
	data = 0x00;
	NRF24_Write_Register(EN_RXADDR, data);
	//5 byte address width.
	data = 0x03;
	NRF24_Write_Register(SETUP_AW, data);
	//We don't need to set SETUP_RETR
	data = 0;
	NRF24_Write_Register(SETUP_RETR, data);
	//RF channel
	NRF24_Write_Register(RF_CH, channel);
	//0dBm - Air data rate = 250Kbps
	data = 0b00100111;
	NRF24_Write_Register(RF_SETUP, data);
	//Tx address setting
	NRF24_Multi_Write_Register(TX_ADDR, address, 5);
	//PWR_UP set.
	data = NRF24_Read_Register(CONFIG);
	data |= (1<<1);
	NRF24_Write_Register(CONFIG, data);
	//need 2ms for transit to standby mode
	delay_us(2000);

}

/*	"If the tx_ds bit is 0, it means no data has been transmitted. The data is written to the TX FIFO,
 * 	and the CE pin is activated for 1ms. This way, the data is transmitted.
 * 	When the data is sent, the tx_ds bit becomes 1. 	*/

void NRF24_Transmit(uint8_t* data){

	uint8_t status_val = 0;

	status_val = NRF24_Read_Register(STATUS);

	if(!(status_val & (1<<5))){
		uint8_t cmd = W_TX_PAYLOAD;
		CSN_pin(LOW);
		HAL_SPI_Transmit(NRF24_SPI, &cmd, 1, 1000);
		HAL_SPI_Transmit(NRF24_SPI, data, 32, 1000);
		CSN_pin(HIGH);
		CE_pin(HIGH);
	    delay_us(1000);//keep HIGH CE pin for 1ms
		CE_pin(LOW);
	}
	else{
		NRF24_Clear_TX_DS();
	}

}







