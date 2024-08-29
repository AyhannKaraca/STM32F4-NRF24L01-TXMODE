# STM32F4-NRF24L01-TXMODE
Configuring the NRF24L01 module as a transmitter using the STM32F4 board.

I configured the NRF24L01 module as a transmitter with the STM32F407 DISC board and set up the same module as a receiver with an Arduino Uno, aiming to establish communication between the two. The code on the Arduino side was copied from a well-known library, while the code on the STM32 side was written by me. The main goal here was to use and better understand the SPI protocol. The module was configured in the simplest way. You can configure extra features (such as Enhanced ShockBurst and AutoAck) to ensure data transmission security or reliability.

This code was written to transmit only two channels of data. It can be easily extended using the same method. Data is collected using ADC with DMA and placed into the buffer for transmission after making some adjustments. Similarly, the receiver side requires some processing to read the data, which is explained in the code comments.

Popular NRF24L01 library for Arduino: [github.com/nRF24/RF24](https://github.com/nRF24/RF24)

If you want to do something much better: https://www.edwinfairchild.com/2020/06/nrf24l01-driver-intro_18.html

Arduino code: https://www.rcpano.net/2022/10/12/uzun-menzilli-uzaktan-kumanda-yapimi-diy-arduino-rc-1-8-kanal-bolum-1/
