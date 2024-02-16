#ifndef UARTCONFIG_H
#define UARTCONFIG_H

enum BaudRates : unsigned long {
  BAUD_4800 = 4800,
  BAUD_9600 = 9600,
  BAUD_19200 = 19200,
  BAUD_31250 = 31250,
  BAUD_38400 = 38400,
  BAUD_57600 = 57600,
  BAUD_74880 = 74880,
  BAUD_115200 = 115200
};

enum DataBits {
  DATA_5 = 5,
  DATA_6 = 6,
  DATA_7 = 7,
  DATA_8 = 8,
};

enum StopBits {
  STOP_1 = 1,
  STOP_2 = 3
};

enum ParityBits {
  PARITY_NONE = 0,
  PARITY_ODD = 1,
  PARITY_EVEN = 2
};

struct UartConfig {
  BaudRates baudRate;
  DataBits dataBits;
  StopBits stopBits;
  ParityBits parityBits;
};

#endif