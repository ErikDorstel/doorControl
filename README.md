# doorControl
ESP32-S3 based door control with Ethernet and RS422 communication
#### Features
* up to eight RC522 doorStations via RS422
* implements a telnet server on port 23
#### RS422
* GPIO 1 Rx1
* GPIO 2 Tx1
* GPIO 4 Rx2
* GPIO 5 Tx2
* GPIO 6 Rx3
* GPIO 7 Tx3
* GPIO 16 Rx4
* GPIO 17 Tx4
* GPIO 18 Rx5
* GPIO 21 Tx5
* GPIO 38 Rx6
* GPIO 39 Tx6
* GPIO 40 Rx7
* GPIO 41 Tx7
* GPIO 42 Rx8
* GPIO 47 Tx8
#### DS3231 + MCP23017
* GPIO 8 SDA
* GPIO 9 SCL
* GPIO 48 RST
#### Eth W5500
* GPIO 10 CS
* GPIO 11 MOSI
* GPIO 12 CLK
* GPIO 13 MISO
* GPIO 14 INT
* GPIO 15 RST